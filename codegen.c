/* codegen --- code generation routines                     2022-08-12 */
/* Copyright (c) 2022 John Honniball. All rights reserved              */

#include <stdio.h>

#include "codegen.h"

static int NextLabel = 0;
static FILE *Asm = NULL;


/* OpenAssemblerFile --- open the output file and write a header */

int OpenAssemblerFile(const char fname[])
{
   Asm = fopen("a.asm", "w");
   
   fprintf(Asm, "        setdp 0\n");
   fprintf(Asm, "        org   $0400\n");
   fprintf(Asm, "appEntry\n");

   return (1);
}


/* CloseAssemblerFile --- write a trailer and close the output file */

int CloseAssemblerFile(void)
{
   fprintf(Asm, "        end  appEntry\n");

   fclose(Asm);
   
   return (1);
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


/* EmitStaticInt --- emit declaration for a local static int variable */

int EmitStaticInt(const int label, const int init, const char comment[])
{
   fprintf(Asm, "l%04d   fdb  %d     ; %s\n", label, init, comment);
}


/* EmitStaticChar --- emit declaration for a local static char variable */

int EmitStaticChar(const int label, const int init, const char comment[])
{
   fprintf(Asm, "l%04d   fcb  %d     ; %s\n", label, init, comment);
}


/* EmitJump --- emit a jump (possibly relative) to a given label */

int EmitJump(const int label, const char comment[])
{
   char target[8];
   
   snprintf(target, sizeof (target), "l%04d", label);
   
   Emit("jmp", target, comment);
}

