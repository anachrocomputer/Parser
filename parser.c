/* parser --- C parser                                      2022-07-31 */
/* Copyright (c) 2022 John Honniball. All rights reserved              */

#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#include "codegen.h"
#include "lexical.h"


void initialise(void);
void parse(const char fname[]);
void parser(const char fname[]);
int ParseDeclaration(struct Token *tok);


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

   GetToken(&tok);
   PrintToken(&tok);

   while (ParseDeclaration(&tok) != EOF)
      ;
      
   EmitLabel(l1);
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


/* ParseDeclaration --- parse a top-level declaration */

int ParseDeclaration(struct Token *tok)
{
   char name[256];
   int type;
   int pLevel;
   
   name[0] = '\0';
   type = 0;
   pLevel = 0;
   
   switch (tok->token) {
   case TEOF:
      return (EOF);
   case TSEMI:
      break;
   case TVOID:
   case TINT:
   case TCHAR:
   case TFLOAT:
   case TDOUBLE:
      type = tok->token;

      GetToken(tok);
      PrintToken(tok);

      while (tok->token == TSTAR) {
         pLevel++;
         GetToken(tok);
         PrintToken(tok);
      }
      
      if (tok->token == TID) {
         strcpy(name, tok->str);
         GetToken(tok);
         PrintToken(tok);
      }
      else {
         fprintf(stderr, "Missing identifier in declaration\n");
      }

      switch (tok->token) {
      case TASSIGN:  // Scalar initialiser
         GetToken(tok);
         PrintToken(tok);
         if (pLevel == 0) {
            switch (type) {
            case TCHAR:
               printf("Initialised char '%s' '%s'\n", name, tok->str);
               EmitExternChar(name, tok->iValue, "char");
               break;
            case TINT:
               printf("Initialised int '%s' '%s'\n", name, tok->str);
               EmitExternInt(name, tok->iValue, "int");
               break;
            case TFLOAT:
               printf("Initialised float '%s' '%s'\n", name, tok->str);
               EmitExternFloat(name, tok->fValue, "float");
               break;
            case TDOUBLE:
               printf("Initialised double '%s' '%s'\n", name, tok->str);
               EmitExternDouble(name, tok->fValue, "double");
               break;
            }
         }
         else {
            printf("Initialised pointer '%s' '%s'\n", name, tok->str);
            EmitExternPointer(name, tok->iValue, "pointer");
         }
         GetToken(tok);
         PrintToken(tok);
         break;
      case TOSQBRK:  // Array
         break;
      case TOPAREN:  // Function
         GetToken(tok);
         PrintToken(tok);
         if (tok->token != TCPAREN) {
            fprintf(stderr, "Malformed function declaration");
         }
         else {
            printf("Function '%s()'\n", name);
         }
         GetToken(tok);
         PrintToken(tok);
         break;
      case TSEMI:    // Uninitialised scalar
         if (pLevel == 0) {
            switch (type) {
            case TCHAR:
               printf("Uninitialised char '%s'\n", name);
               EmitExternChar(name, 0, "char");
               break;
            case TINT:
               printf("Uninitialised int '%s'\n", name);
               EmitExternInt(name, 0, "int");
               break;
            case TFLOAT:
               printf("Uninitialised float '%s'\n", name);
               EmitExternFloat(name, 0.0f, "float");
               break;
            case TDOUBLE:
               printf("Uninitialised double '%s'\n", name);
               EmitExternDouble(name, 0.0, "double");
               break;
            }
         }
         else {
            printf("Uninitialised pointer '%s'\n", name);
            EmitExternPointer(name, 0, "pointer");
         }
         break;
      default:
         fprintf(stderr, "Unexpected symbol '%s' in declaration\n", tok->str);
         break;
      }
      
      break;
   default:
      fprintf(stderr, "Unexpected symbol '%s' in declaration\n", tok->str);
      break;
   }

   if (tok->token != TSEMI) {
      fprintf(stderr, "Missing semicolon in declaration\n");
   }
   
   GetToken(tok);
   PrintToken(tok);
   
   return (tok->token);
}

