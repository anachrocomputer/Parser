/* parser --- C parser                                      2022-07-31 */
/* Copyright (c) 2022 John Honniball. All rights reserved              */

#include <stdio.h>
#include <stdbool.h>

#include "codegen.h"
#include "lexical.h"


void initialise(void);
void parse(const char fname[]);
void parser(const char fname[]);


int main(const int argc, const char *argv[])
{
   int i;
   
   initialise();
   
   for (i = 1; i < argc; i++) {
      if (argv[i][0] == '-') {
      }
      else {
         parse(argv[i]);
      }
   }

   return (0);
}


/* initialise --- call all module initialisation functions */

void initialise(void)
{
   CodeGenInit();
   LexicalInit();
}


/* parse --- open, parse and translate a single source code file */

void parse(const char fname[])
{
   if (OpenSourceFile(fname) == false)
      return;
      
   if (OpenAssemblerFile(fname) == false) {
      CloseSourceFile();
      return;
   }
   
   parser(fname);

   CloseAssemblerFile();
   CloseSourceFile();
}


void parser(const char fname[])
{
   struct Token tok;
   const int l1 = AllocLabel('L');
   const int l2 = AllocLabel('L');
   const int s1 = AllocLabel('L');
   const int s2 = AllocLabel('I');
   const int s3 = AllocLabel('C');

   EmitLabel(l1);
   
   while (GetToken(&tok) != EOF)
      PrintToken(&tok);
   /* ParseDeclaration(); */
      
   
   Emit("nop", "", "Do nothing");
   EmitJump(l2, "Jump forward to l2");
   Emit("nop", "", "Do nothing");
   EmitLabel(l2);
   LoadIntConstant(42, 'D', "Load constant 42");
   StoreStaticInt(s2, 'D', "Store 'state'");
   StoreExternInt("Wellerman", 'D', "Store 'Wellerman'");
   LoadExternInt("Wellerman", 'D', "Load 'Welleman'");
   LoadStaticChar(s3, 'D', true, "Load 'letter'");
   LoadStaticInt(s2, 'X', "Load 'state'");
   LoadStaticLong(s1, "Load 'bytes'");
   EmitJump(l1, "Jump back to l1");
   Emit("rts", "", "Return to caller");
   EmitExternInt("Wellerman", 42, "int Wellerman");
   EmitStaticLong(s1, 65536L, "bytes");
   EmitStaticInt(s2, 42, "state");
   EmitStaticChar(s3, 'A', "letter");
}

