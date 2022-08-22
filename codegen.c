/* codegen --- code generation routines                     2022-08-12 */
/* Copyright (c) 2022 John Honniball. All rights reserved              */

#include <stdio.h>
#include <stdbool.h>

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
   Asm = fopen("a.asm", "w");
   
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


/* EmitLabel --- emit a label into the assembly-language file */

void EmitLabel(const int label)
{
   fprintf(Asm, "l%04d\n", label);
}


/* EmitStaticLong --- emit declaration for a local static long int variable */

int EmitStaticLong(const int label, const long int init, const char comment[])
{
   fprintf(Asm, "l%04d   fqb  %ld     ; %s\n", label, init, comment);
}


/* LoadStaticLong --- load a static long int into register Q */

int LoadStaticLong(const int label, const char comment[])
{
   char target[8];

   snprintf(target, sizeof (target), "l%04d", label);
   
   Emit("ldq", target, comment);
}


/* EmitStaticInt --- emit declaration for a local static int variable */

int EmitStaticInt(const int label, const int init, const char comment[])
{
   fprintf(Asm, "l%04d   fdb  %d     ; %s\n", label, init, comment);
}


/* LoadIntConstant --- load an int constant into a given register */

int LoadIntConstant(const int val, const int reg, const char comment[])
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


/* EmitExternChar --- emit declaration for an extern char variable */

int EmitExternChar(const char name[], const int init, const char comment[])
{
   fprintf(Asm, "%c%-30s fcb  %d     ; %s\n", NAME_PREFIX, name, init, comment);
}


/* EmitExternInt --- emit declaration for an extern int variable */

int EmitExternInt(const char name[], const int init, const char comment[])
{
   fprintf(Asm, "%c%-30s fdb  %d     ; %s\n", NAME_PREFIX, name, init, comment);
}


/* EmitExternPointer --- emit declaration for an extern pointer variable */

int EmitExternPointer(const char name[], const int init, const char comment[])
{
   fprintf(Asm, "%c%-30s fdb  %d     ; %s\n", NAME_PREFIX, name, init, comment);
}


/* EmitExternFloat --- emit declaration for an extern float variable */

int EmitExternFloat(const char name[], const float init, const char comment[])
{
   int b1, b2, b3, b4;
   
   b1 = 0;  // TODO: generate IEEE-754 'float' here
   b2 = 0;
   b3 = 0;
   b4 = 0;

   fprintf(Asm, "%c%-30s fcb  %d,%d,%d,%d ; %g %s\n", NAME_PREFIX, name, b1, b2, b3, b4, init, comment);
}


/* EmitExternDouble --- emit declaration for an extern double variable */

int EmitExternDouble(const char name[], const double init, const char comment[])
{
   int b1, b2, b3, b4, b5, b6, b7, b8;
   
   b1 = 0;  // TODO: generate IEEE-754 'double' here
   b2 = 0;
   b3 = 0;
   b4 = 0;
   b5 = 0;
   b6 = 0;
   b7 = 0;
   b8 = 0;

   fprintf(Asm, "%c%-30s fcb  %d,%d,%d,%d,%d,%d,%d,%d ; %g %s\n", NAME_PREFIX, name, b1, b2, b3, b4, b5, b6, b7, b8, init, comment);
}


/* LoadExternInt --- load an extern int into a given register */

int LoadExternInt(const char name[], const int reg, const char comment[])
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

int StoreExternInt(const char name[], const int reg, const char comment[])
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

int LoadStaticInt(const int label, const int reg, const char comment[])
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

int StoreStaticInt(const int label, const int reg, const char comment[])
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

int EmitStaticChar(const int label, const int init, const char comment[])
{
   fprintf(Asm, "l%04d   fcb  %d     ; %s\n", label, init, comment);
}


/* LoadStaticChar --- load a local static char variable into a register */

int LoadStaticChar(const int label, const int reg, const bool signExtend, const char comment[])
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

int EmitJump(const int label, const char comment[])
{
   char target[8];
   
   snprintf(target, sizeof (target), "l%04d", label);
   
   Emit("jmp", target, comment);
}

