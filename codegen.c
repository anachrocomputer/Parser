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

   Emit("pshs", "u", "Save old frame pointer");
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
   Emit("puls", "u", "Restore frame pointer");
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


/* EmitStaticLong --- emit declaration for a local static long int variable */

void EmitStaticLong(const int label, const long int init, const char comment[])
{
   fprintf(Asm, "l%04d   fqb  %ld       ; static long int %s = %ld\n", label, init, comment, init);
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


/* LoadScalar --- load a scalar variable into D or Q */

void LoadScalar(const struct Symbol *const sym, char comment[])
{
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


/* StoreScalar --- store a scalar variable from D or Q */

void StoreScalar(const struct Symbol *const sym, char comment[])
{
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


/* EmitStaticInt --- emit declaration for a local static int variable */

void EmitStaticInt(const int label, const int init, const char comment[])
{
   fprintf(Asm, "l%04d   fdb  %d               ; static int %s = %d\n", label, init, comment, init);
}


/* EmitStaticFloat --- emit declaration for a local static float variable */

void EmitStaticFloat(const int label, const float init, const char comment[])
{
   int b1, b2, b3, b4;
   union {
      float f;
      unsigned char b[4];
   } v;
   
   v.f = init;
   
   b1 = v.b[3];   // 6809 is big-endian
   b2 = v.b[2];
   b3 = v.b[1];
   b4 = v.b[0];

   fprintf(Asm, "l%04d   fcb  %d,%d,%d,%d         ; static float %s = %g\n", label, b1, b2, b3, b4, comment, init);
}


/* EmitStaticDouble --- emit declaration for a local static double variable */

void EmitStaticDouble(const int label, const double init, const char comment[])
{
   int b1, b2, b3, b4, b5, b6, b7, b8;
   union {
      double d;
      unsigned char b[4];
   } v;
   
   v.d = init;
   
   b1 = v.b[7];   // 6809 is big-endian
   b2 = v.b[6];
   b3 = v.b[5];
   b4 = v.b[4];
   b5 = v.b[3];
   b6 = v.b[2];
   b7 = v.b[1];
   b8 = v.b[0];

   fprintf(Asm, "l%04d   fcb  %d,%d,%d,%d,%d,%d,%d,%d ; static double %s = %g\n", label, b1, b2, b3, b4, b5, b6, b7, b8, comment, init);
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
   }
   else {
      fprintf(stderr, "Only 'extern' or 'static' is valid\n");
   }
   
   if (sym->pLevel == 0) {
      switch (sym->type) {
      case T_CHAR:
      case T_UCHAR:
         fprintf(Asm, "%-30s  fcb  %d      ; char %s = %d\n", name, init, sym->name, init);
         break;
      case T_SHORT:
      case T_USHORT:
      case T_INT:
      case T_UINT:
         fprintf(Asm, "%-30s  fdb  %d      ; int %s = %d\n", name, init, sym->name, init);
         break;
      case T_LONG:
      case T_ULONG:
         fprintf(Asm, "%-30s  fqb  %d      ; long int %s = %d\n", name, init, sym->name, init);
         break;
      case T_FLOAT:
         f.f = fInit;
         
         b1 = f.b[3];   // 6809 is big-endian
         b2 = f.b[2];
         b3 = f.b[1];
         b4 = f.b[0];

         fprintf(Asm, "%-30s  fcb  %d,%d,%d,%d         ; float %s = %g\n", name, b1, b2, b3, b4, sym->name, fInit);
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

         fprintf(Asm, "%-30s  fcb  %d,%d,%d,%d,%d,%d,%d,%d ; double %s = %g\n", name, b1, b2, b3, b4, b5, b6, b7, b8, sym->name, fInit);
         break;
      }
   }
   else {
      fprintf(Asm, "%-30s  fdb  %d     ; pointer %s = %d\n", name, init, sym->name, init);
   }
}


/* EmitStaticChar --- emit declaration for a local static char variable */

void EmitStaticChar(const int label, const int init, const char comment[])
{
   fprintf(Asm, "l%04d   fcb  %d               ; static char %s = %d\n", label, init, comment, init);
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
   char target[MAXNAME + 1];
   char op[30];

   GenTargetOperand(sym, 0, target);
   snprintf(op, sizeof (op), "%d,x", amount);

   switch (sym->type) {
   case T_CHAR:
   case T_UCHAR:
      if (amount == 1) {
         Emit("inc", target, "inc char");
      }
      else if (amount == -1) {
         Emit("dec", target, "dec char");
      }
      else {
         fprintf(stderr, "Inc/Dec by more than 1\n");
      }
      break;
   case T_SHORT:
   case T_USHORT:
   case T_INT:
   case T_UINT:
      Emit("ldx", target, "inc");
      Emit("leax", op, "inc");
      Emit("stx", target, "inc");
      break;
   case T_LONG:
   case T_ULONG:
      Emit("nop", "", "inc long");
      break;
   case T_FLOAT:
      Emit("nop", "", "inc float");
      break;
   case T_DOUBLE:
      Emit("nop", "", "inc double");
      break;
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

