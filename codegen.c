/* codegen --- code generation routines                     2022-08-12 */
/* Copyright (c) 2022 John Honniball. All rights reserved              */

#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#include "codegen.h"

#define NAME_PREFIX  ('_')

static int NextLabel = 0;
static FILE *Asm = NULL;


/* CodeGenInit --- initialise this module */

void CodeGenInit(void)
{
}


/* OpenAssemblerFile --- open the output file and write a header */

bool OpenAssemblerFile(const char fname[])
{
   char asmName[256];
   char *p;
   
   strncpy(asmName, fname, sizeof (asmName) - 2);
   p = strrchr(asmName, '.');
   strcpy(p, ".asm");
   
   Asm = fopen(asmName, "w");
   
   fprintf(Asm, "        setdp 0\n");
   fprintf(Asm, "        org   $0400\n");
   fprintf(Asm, "appEntry jmp  _main\n");
   fprintf(Asm, "_vduchar tfr  b,a               ; Move char into A register\n");
   fprintf(Asm, "         jmp  $a020             ; Send one char to the VDU\n");
   fprintf(Asm, "_vdustr  tfr  d,x               ; Move pointer into X register\n");
   fprintf(Asm, "         jmp  $a014             ; Print a nul-terminated string\n");
   fprintf(Asm, "_getchar jsr  $a252             ; Get char and show cursor\n");
   fprintf(Asm, "         tfr  a,b               ; Move returned ASCII char into LSB of D register\n");
   fprintf(Asm, "         clra                   ; Make sure MSB is zero\n");
   fprintf(Asm, "         rts\n");

   return (true);
}


/* CloseAssemblerFile --- write a trailer and close the output file */

bool CloseAssemblerFile(void)
{
   fprintf(Asm, "        end  appEntry\n");

   fclose(Asm);
   
   return (true);
}


/* Emit --- emit a single assembly-language instruction */

int Emit(const char inst[], const char oper[], const char comment[])
{
   fprintf(Asm, "        %-4s %-32s ; %s\n", inst, oper, comment);

   return (1);
}


/* AllocLabel --- allocate a new label for a given purpose */

int AllocLabel(const char purpose)
{
   return (NextLabel++);
}


/* EmitLabel --- emit an allocated label into the assembly-language file */

void EmitLabel(const int label)
{
   fprintf(Asm, "l%04d\n", label);
}


/* EmitFunctionEntry --- emit a label and setup code for a function */

void EmitFunctionEntry(const char name[], const int nBytes)
{
   fprintf(Asm, "%c%-44s ; Function entry point\n", NAME_PREFIX, name);

   Emit("pshs", "u,y", "Save old frame pointer & register variable");
   Emit("tfr", "s,u", "Make new frame pointer");
   
   if (nBytes != 0) {
      char frame[32];
      
      snprintf(frame, sizeof (frame), "-%d,s", nBytes);
      Emit("leas", frame, "Allocate stack frame");
   }
}


/* EmitFunctionExit --- emit return label and function exit code */

void EmitFunctionExit(const int returnLabel)
{
   EmitLabel(returnLabel);
   Emit("tfr", "u,s", "Deallocate stack frame");
   Emit("puls", "u,y", "Restore frame pointer & register variable");
   Emit("rts", "", "Return to caller");
}


/* EmitStaticCharArray --- emit declaration for an initialised char array or string */

void EmitStaticCharArray(const struct StringConstant *sc, const char name[])
{
   char target[8];
   char bytes[64];
   int i, n;

   snprintf(target, sizeof (target), "l%04d", sc->label);
   
   bytes[0] = '\0';
   
   if (sc->sLength > 7) {
      n = 7;
   }
   else {
      n = sc->sLength;
   }

   for (i = 0; i < n; i++) {
      char hex[8];
      
      sprintf(hex, "$%02x", sc->sValue[i] & 0xff);
      
      strcat(bytes, hex);
      
      if (i < (n - 1)) {
         strcat(bytes, ",");
      }
   }
   
   fprintf(Asm, "%-7s fcb  %-32s ; const char %s[%d] = %s\n", target, bytes, name, sc->sLength - 1, sc->str);
   
   while (i < sc->sLength) {
      n += 7;
      
      if (n > sc->sLength) {
         n = sc->sLength;
      }

      bytes[0] = '\0';

      for ( ; i < n; i++) {
         char hex[8];

         sprintf(hex, "$%02x", sc->sValue[i] & 0xff);
         
         strcat(bytes, hex);
         
         if (i < (n - 1)) {
            strcat(bytes, ",");
         }

      }

      fprintf(Asm, "        fcb  %s\n", bytes);
   }
}


/* GenTargetOperand --- generate the assembler operand to address a scalar variable */

static void GenTargetOperand(const struct Symbol *const sym, const int offset, char target[])
{
   if (offset == 0) {
      switch (sym->storageClass) {
      case SCEXTERN:
         snprintf(target, MAXNAME + 1, "%c%s", NAME_PREFIX, sym->name);
         break;
      case SCSTATIC:
         snprintf(target, MAXNAME + 1, "l%04d", sym->label);
         break;
      case SCAUTO:
         snprintf(target, MAXNAME + 1, "%d,u", sym->fpOffset);
         break;
      case SCREGISTER:
         snprintf(target, MAXNAME + 1, "#0");
         break;
      }
   }
   else {
      switch (sym->storageClass) {
      case SCEXTERN:
         snprintf(target, MAXNAME + 1, "%c%s+%d", NAME_PREFIX, sym->name, offset);
         break;
      case SCSTATIC:
         snprintf(target, MAXNAME + 1, "l%04d+%d", sym->label, offset);
         break;
      case SCAUTO:
         snprintf(target, MAXNAME + 1, "%d,u", sym->fpOffset + offset);
         break;
      case SCREGISTER:
         snprintf(target, MAXNAME + 1, "#0");
         break;
      }
   }
}



/* storageClassAsString --- generate a string representation of a storage class */

static char *storageClassAsString(const int sc)
{
   char *str = "";
   
   switch (sc) {
   case SCEXTERN:
      str = "extern";
      break;
   case SCSTATIC:
      str = "static";
      break;
   case SCAUTO:
      str = "auto";
      break;
   case SCREGISTER:
      str = "register";
      break;
   }
   
   return (str);
}


/* typeAsString --- generate a string representation of a type */

static char *typeAsString(const int ty)
{
   char *str = "";
   
   switch (ty) {
   case T_CHAR:
      str = "char";
      break;
   case T_UCHAR:
      str = "unsigned char";
      break;
   case T_INT:
      str = "int";
      break;
   case T_UINT:
      str = "unsigned int";
      break;
   case T_FLOAT:
      str = "float";
      break;
   case T_DOUBLE:
      str = "double";
      break;
   }
   
   return (str);
}


/* LoadScalar --- load a scalar variable into D or Q */

void LoadScalar(const struct Symbol *const sym)
{
   char comment[MAXNAME + 64];
   const char *sc = storageClassAsString(sym->storageClass);
   const char *ty = typeAsString(sym->type);
   
   snprintf(comment, sizeof (comment), "Load %s %s %s", sc, ty, sym->name);
   
   if (sym->storageClass == SCREGISTER) {
      switch (sym->type) {
      case T_CHAR:
         Emit("tfr", "y,d", comment);
         Emit("sex", "", "Sign extend to 16 bits");
         break;
      case T_UCHAR:
         Emit("tfr", "y,d", comment);
         Emit("clra", "", "No sign extension");
         break;
      case T_SHORT:
      case T_USHORT:
      case T_INT:
      case T_UINT:
         Emit("tfr", "y,d", comment);
         break;
      }
   }
   else {
      char target[MAXNAME + 1];

      GenTargetOperand(sym, 0, target);

      switch (sym->type) {
      case T_CHAR:
         Emit("ldb", target, comment);
         Emit("sex", "", "Sign extend to 16 bits");
         break;
      case T_UCHAR:
         Emit("ldb", target, comment);
         Emit("clra", "", "No sign extension");
         break;
      case T_SHORT:
      case T_USHORT:
      case T_INT:
      case T_UINT:
         Emit("ldd", target, comment);
         break;
      case T_LONG:
      case T_ULONG:
         Emit("ldq", target, comment);
         break;
      case T_FLOAT:
         Emit("ldq", target, comment);
         break;
      case T_DOUBLE:
         Emit("ldq", target, comment);
         Emit("nop", "", "How to load low 32 bits?");
         break;
      }
   }
}


/* StoreScalar --- store a scalar variable from D or Q */

void StoreScalar(const struct Symbol *const sym)
{
   char comment[MAXNAME + 64];
   const char *sc = storageClassAsString(sym->storageClass);
   const char *ty = typeAsString(sym->type);
   
   snprintf(comment, sizeof (comment), "Store %s %s %s", sc, ty, sym->name);

   if (sym->storageClass == SCREGISTER) {
      switch (sym->type) {
      case T_CHAR:
      case T_UCHAR:
         Emit("tfr", "d,y", comment);
         break;
      case T_SHORT:
      case T_USHORT:
      case T_INT:
      case T_UINT:
         Emit("tfr", "d,y", comment);
         break;
      }
   }
   else {
      char target[MAXNAME + 1];

      GenTargetOperand(sym, 0, target);

      switch (sym->type) {
      case T_CHAR:
      case T_UCHAR:
         Emit("stb", target, comment);
         break;
      case T_SHORT:
      case T_USHORT:
      case T_INT:
      case T_UINT:
         Emit("std", target, comment);
         break;
      case T_LONG:
      case T_ULONG:
         Emit("stq", target, comment);
         break;
      case T_FLOAT:
         Emit("stq", target, comment);
         break;
      case T_DOUBLE:
         Emit("stq", target, comment);
         GenTargetOperand(sym, 4, target);
         Emit("stq", target, "Store low 32 bits");
         break;
      }
   }
}


/* LoadIntConstant --- load an int constant into a given register */

void LoadIntConstant(const int val, const int reg, const char comment[])
{
   char immediate[8];

   snprintf(immediate, sizeof (immediate), "#%d", val);

   switch (reg) {
   case 'D':
   case 'd':
      Emit("ldd", immediate, comment);
      break;
   case 'X':
   case 'x':
      Emit("ldx", immediate, comment);
      break;
   case 'Y':
   case 'y':
      Emit("ldy", immediate, comment);
      break;
   }
}


/* LoadLabelAddr --- load the address of a generated label */

void LoadLabelAddr(const int label, const char comment[])
{
   char target[8];

   snprintf(target, sizeof (target), "#l%04d", label);
   
   Emit("ldd", target, comment);
}


/* EmitExternScalar --- emit declaration for an extern scalar variable */

void EmitExternScalar(const struct Symbol *const sym, const int init, const double fInit)
{
   char name[MAXNAME + 1];
   char *storage = "";
   int b1, b2, b3, b4, b5, b6, b7, b8;
   union {
      float f;
      unsigned char b[4];
   } f;
   union {
      double d;
      unsigned char b[4];
   } d;
   
   if (sym->storageClass == SCEXTERN) {
      snprintf(name, sizeof (name), "%c%s", NAME_PREFIX, sym->name);
   }
   else if (sym->storageClass == SCSTATIC) {
      snprintf(name, sizeof (name), "l%04d", sym->label);
      storage = "static ";
   }
   else {
      fprintf(stderr, "Only 'extern' or 'static' is valid\n");
   }
   
   if (sym->pLevel == 0) {
      switch (sym->type) {
      case T_CHAR:
      case T_UCHAR:
         fprintf(Asm, "%-30s  fcb  %d      ; %schar %s = %d\n", name, init, storage, sym->name, init);
         break;
      case T_SHORT:
      case T_USHORT:
      case T_INT:
      case T_UINT:
         fprintf(Asm, "%-30s  fdb  %d      ; %sint %s = %d\n", name, init, storage, sym->name, init);
         break;
      case T_LONG:
      case T_ULONG:
         fprintf(Asm, "%-30s  fqb  %d      ; %slong int %s = %d\n", name, init, storage, sym->name, init);
         break;
      case T_FLOAT:
         f.f = fInit;
         
         b1 = f.b[3];   // 6809 is big-endian
         b2 = f.b[2];
         b3 = f.b[1];
         b4 = f.b[0];

         fprintf(Asm, "%-30s  fcb  %d,%d,%d,%d         ; %sfloat %s = %g\n", name, b1, b2, b3, b4, storage, sym->name, fInit);
         break;
      case T_DOUBLE:
         d.d = fInit;
         
         b1 = d.b[7];   // 6809 is big-endian
         b2 = d.b[6];
         b3 = d.b[5];
         b4 = d.b[4];
         b5 = d.b[3];
         b6 = d.b[2];
         b7 = d.b[1];
         b8 = d.b[0];

         fprintf(Asm, "%-30s  fcb  %d,%d,%d,%d,%d,%d,%d,%d ; %sdouble %s = %g\n", name, b1, b2, b3, b4, b5, b6, b7, b8, storage, sym->name, fInit);
         break;
      }
   }
   else {
      fprintf(Asm, "%-30s  fdb  %d     ; %spointer %s = %d\n", name, init, storage, sym->name, init);
   }
}


/* EmitJump --- emit a jump (possibly relative) to a given label */

void EmitJump(const int label, const char comment[])
{
   char target[8];
   
   snprintf(target, sizeof (target), "l%04d", label);
   
   Emit("jmp", target, comment);
}


/* EmitBranchIfEqual --- emit a BEQ to a label */

void EmitBranchIfEqual(const int label, const char comment[])
{
   char target[8];
   
   snprintf(target, sizeof (target), "l%04d", label);
   
   Emit("lbeq", target, comment);
}


/* EmitBranchNotEqual --- emit a BNE to a label */

void EmitBranchNotEqual(const int label, const char comment[])
{
   char target[8];
   
   snprintf(target, sizeof (target), "l%04d", label);
   
   Emit("lbne", target, comment);
}


/* EmitIncScalar --- emit an INC for a scalar variable */

void EmitIncScalar(const struct Symbol *const sym, const int amount)
{
   char op[30];
   char comment[MAXNAME + 64];
   const char *sc = storageClassAsString(sym->storageClass);
   const char *ty = typeAsString(sym->type);
   char *incDec = "inc";
   
   if (amount < 0) {
      incDec = "dec";
   }

   snprintf(comment, sizeof (comment), "%s %s %s %s", incDec, sc, ty, sym->name);


   if (sym->storageClass == SCREGISTER) {
      snprintf(op, sizeof (op), "%d,y", amount);

      switch (sym->type) {
      case T_CHAR:
      case T_UCHAR:
      case T_SHORT:
      case T_USHORT:
      case T_INT:
      case T_UINT:
         Emit("leay", op, comment);
         break;
      }
   }
   else {
      char target[MAXNAME + 1];

      GenTargetOperand(sym, 0, target);

      snprintf(op, sizeof (op), "%d,x", amount);

      switch (sym->type) {
      case T_CHAR:
      case T_UCHAR:
         if (amount == 1) {
            Emit("inc", target, comment);
         }
         else if (amount == -1) {
            Emit("dec", target, comment);
         }
         else {
            fprintf(stderr, "Inc/Dec by more than 1\n");
         }
         break;
      case T_SHORT:
      case T_USHORT:
      case T_INT:
      case T_UINT:
         Emit("ldx", target, comment);
         Emit("leax", op, incDec);
         Emit("stx", target, comment);
         break;
      case T_LONG:
      case T_ULONG:
         Emit("nop", "", comment);
         break;
      case T_FLOAT:
         Emit("nop", "", comment);
         break;
      case T_DOUBLE:
         Emit("nop", "", comment);
         break;
      }
   }
}


/* EmitCompareIntConstant --- code to compare an integer constant */

void EmitCompareIntConstant(const int compare, const char comment[])
{
   char cmpArg[16];
   
   snprintf(cmpArg, sizeof (cmpArg), "#%d", compare);
   
   Emit("cmpd", cmpArg, comment);
}


/* EmitCallFunction --- code to call a function */

void EmitCallFunction(const char name[], const char comment[])
{
   char target[30];

   snprintf(target, sizeof (target), "%c%s", NAME_PREFIX, name);

   Emit("jsr", target, comment);
}

