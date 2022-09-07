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
   fprintf(Asm, "appEntry\n");

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


/* LoadStaticLong --- load a static long int into register Q */

void LoadStaticLong(const int label, const char comment[])
{
   char target[8];

   snprintf(target, sizeof (target), "l%04d", label);
   
   Emit("ldq", target, comment);
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


/* EmitExternChar --- emit declaration for an extern char variable */

void EmitExternChar(const char name[], const int init, const char comment[])
{
   fprintf(Asm, "%c%-30s fcb  %d     ; %s\n", NAME_PREFIX, name, init, comment);
}


/* EmitExternInt --- emit declaration for an extern int variable */

void EmitExternInt(const char name[], const int init, const char comment[])
{
   fprintf(Asm, "%c%-30s fdb  %d     ; %s\n", NAME_PREFIX, name, init, comment);
}


/* EmitExternPointer --- emit declaration for an extern pointer variable */

void EmitExternPointer(const char name[], const int init, const char comment[])
{
   fprintf(Asm, "%c%-30s fdb  %d     ; %s\n", NAME_PREFIX, name, init, comment);
}


/* EmitExternFloat --- emit declaration for an extern float variable */

void EmitExternFloat(const char name[], const float init, const char comment[])
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

   fprintf(Asm, "%c%-30s fcb  %d,%d,%d,%d ; %g %s\n", NAME_PREFIX, name, b1, b2, b3, b4, init, comment);
}


/* EmitExternDouble --- emit declaration for an extern double variable */

void EmitExternDouble(const char name[], const double init, const char comment[])
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
   
   fprintf(Asm, "%c%-30s fcb  %d,%d,%d,%d,%d,%d,%d,%d ; %g %s\n", NAME_PREFIX, name, b1, b2, b3, b4, b5, b6, b7, b8, init, comment);
}


/* LoadExternInt --- load an extern int into a given register */

void LoadExternInt(const char name[], const int reg, const char comment[])
{
   char target[30];

   snprintf(target, sizeof (target), "%c%s", NAME_PREFIX, name);
   
   switch (reg) {
   case 'D':
   case 'd':
      Emit("ldd", target, comment);
      break;
   case 'X':
   case 'x':
      Emit("ldx", target, comment);
      break;
   case 'Y':
   case 'y':
      Emit("ldy", target, comment);
      break;
   }
}


/* StoreExternInt --- store a register into an extern int variable */

void StoreExternInt(const char name[], const int reg, const char comment[])
{
   char target[30];

   snprintf(target, sizeof (target), "%c%s", NAME_PREFIX, name);
   
   switch (reg) {
   case 'D':
   case 'd':
      Emit("std", target, comment);
      break;
   case 'X':
   case 'x':
      Emit("stx", target, comment);
      break;
   case 'Y':
   case 'y':
      Emit("sty", target, comment);
      break;
   }
}


/* LoadStaticInt --- load a static int into a given register */

void LoadStaticInt(const int label, const int reg, const char comment[])
{
   char target[8];

   snprintf(target, sizeof (target), "l%04d", label);
   
   switch (reg) {
   case 'D':
   case 'd':
      Emit("ldd", target, comment);
      break;
   case 'X':
   case 'x':
      Emit("ldx", target, comment);
      break;
   case 'Y':
   case 'y':
      Emit("ldy", target, comment);
      break;
   }
}


/* StoreStaticInt --- store a register into a static int variable */

void StoreStaticInt(const int label, const int reg, const char comment[])
{
   char target[8];

   snprintf(target, sizeof (target), "l%04d", label);
   
   switch (reg) {
   case 'D':
   case 'd':
      Emit("std", target, comment);
      break;
   case 'X':
   case 'x':
      Emit("stx", target, comment);
      break;
   case 'Y':
   case 'y':
      Emit("sty", target, comment);
      break;
   }
}


/* EmitStaticChar --- emit declaration for a local static char variable */

void EmitStaticChar(const int label, const int init, const char comment[])
{
   fprintf(Asm, "l%04d   fcb  %d               ; static char %s = %d\n", label, init, comment, init);
}


/* LoadStaticChar --- load a local static char variable into a register */

void LoadStaticChar(const int label, const int reg, const bool signExtend, const char comment[])
{
   char *mnem = NULL;
   char target[8];
   
   switch (reg) {
   case 'D':
   case 'd':
      mnem = "ldb";
      break;
   }
   
   snprintf(target, sizeof (target), "l%04d", label);

   Emit(mnem, target, comment);
   
   if (signExtend)
      Emit("sex", "", "Sign extend to 16 bits");
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


/* EmitIncExternInt --- emit an INC for an extern int variable */

void EmitIncExternInt(const char name[], const int amount)
{
   char target[30];
   char op[30];

   snprintf(target, sizeof (target), "%c%s", NAME_PREFIX, name);
   snprintf(op, sizeof (op), "%d,x", amount);

   Emit("ldx", target, "inc");
   Emit("leax", op, "inc");
   Emit("stx", target, "inc");
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

