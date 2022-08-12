/* codegen --- code generation routines                     2022-08-12 */

#include <stdio.h>

#include "codegen.h"

static int NextLabel = 0;
static FILE *Asm = NULL;


int OpenAssemblerFile(const char fname[])
{
   Asm = fopen("a.asm", "w");
   
   fprintf(Asm, "        setdp 0\n");
   fprintf(Asm, "        org   $0400\n");
   fprintf(Asm, "appEntry\n");

   return (1);
}


int CloseAssemblerFile(void)
{
   fprintf(Asm, "        end  appEntry\n");

   fclose(Asm);
   
   return (1);
}



int Emit(const char inst[], const char oper[], const char comment[])
{
   fprintf(Asm, "        %-4s %-32s ; %s\n", inst, oper, comment);

   return (1);
}


int AllocLabel(const char purpose)
{
   return (NextLabel++);
}


void EmitLabel(const int label)
{
   fprintf(Asm, "l%04d\n", label);
}


int EmitStaticLong(const int label, const long int init, const char comment[])
{
   fprintf(Asm, "l%04d   fqb  %ld     ; %s\n", label, init, comment);
}

int EmitStaticInt(const int label, const int init, const char comment[])
{
   fprintf(Asm, "l%04d   fdb  %d     ; %s\n", label, init, comment);
}

int EmitStaticChar(const int label, const int init, const char comment[])
{
   fprintf(Asm, "l%04d   fcb  %d     ; %s\n", label, init, comment);
}

int EmitJump(const int label, const char comment[])
{
   char target[8];
   
   snprintf(target, sizeof (target), "l%04d", label);
   
   Emit("jmp", target, comment);
}

