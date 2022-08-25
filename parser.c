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
void ParseFunctionBody(struct Token *tok, const char name[], const int pLevel, const int type);
void ParseStatement(struct Token *tok, const int returnLabel, const int breakLabel, const int continueLabel);
void ParseReturn(struct Token *tok, const int returnLabel);
void ParseCompoundStatement(struct Token *tok, const int returnLabel, const int breakLabel, const int continueLabel);
void ParseIf(struct Token *tok, const int returnLabel);
void ParseExpression(struct Token *tok);
void ParseDo(struct Token *tok, const int returnLabel);
void ParseBreak(struct Token *tok, const int breakLabel);
void ParseContinue(struct Token *tok, const int continueLabel);
void ParseGoto(struct Token *tok);


int main(const int argc, const char *argv[])
{
   int i;
   
   initialise();
   
   for (i = 1; i < argc; i++) {
      if (argv[i][0] == '-') {
         switch (argv[i][1]) {
         case 'T':
            SetTokenTraceFlag(true);
            break;
         }
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

   GetToken(&tok);

   while (ParseDeclaration(&tok) != EOF)
      ;
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

      while (tok->token == TSTAR) {
         pLevel++;
         GetToken(tok);
      }
      
      if (tok->token == TID) {
         strcpy(name, tok->str);
         GetToken(tok);
      }
      else {
         fprintf(stderr, "Missing identifier in declaration\n");
      }

      switch (tok->token) {
      case TASSIGN:  // Scalar initialiser
         GetToken(tok);
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
         break;
      case TOSQBRK:  // Array
         break;
      case TOPAREN:  // Function
         GetToken(tok);
         
         if (tok->token == TVOID) {
            GetToken(tok);
         }
         
         if (tok->token != TCPAREN) {
            fprintf(stderr, "Malformed function declaration");
         }
         else {
            if (pLevel == 0) {
               switch (type) {
               case TVOID:
                  printf("Function '%s()' returning void\n", name);
                  break;
               case TCHAR:
                  printf("Function '%s()' returning char\n", name);
                  break;
               case TINT:
                  printf("Function '%s()' returning int\n", name);
                  break;
               case TFLOAT:
                  printf("Function '%s()' returning float\n", name);
                  break;
               case TDOUBLE:
                  printf("Function '%s()' returning double\n", name);
                  break;
               }
            }
            else {
               printf("Function '%s()' returning pointer\n", name);
            }
         }

         GetToken(tok);

         if (tok->token == TOBRACE) {
            printf("<function_definition>\n");
            ParseFunctionBody(tok, name, pLevel, type);
         }

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

   if (!((tok->token == TSEMI) || (tok->token == TCBRACE))) {
      fprintf(stderr, "Missing semicolon or close curly bracket in declaration\n");
   }
   
   GetToken(tok);
   
   return (tok->token);
}


/* ParseFunctionBody --- parse the body of a function */

void ParseFunctionBody(struct Token *tok, const char name[], const int pLevel, const int type)
{
   int autoSize = 0;
   const int returnLabel = AllocLabel('R');
   
   EmitFunctionLabel(name);
   Emit("pshs", "u", "Save old frame pointer");
   Emit("tfr", "s,u", "Make new frame pointer");
   
   GetToken(tok);
   
   while ((tok->token == TINT) || (tok->token == TCHAR) ||
          (tok->token == TFLOAT) || (tok->token == TDOUBLE)) {
      char name[256];
      int type = tok->token;
      int pLevel = 0;
      
      GetToken(tok);

      while (tok->token == TSTAR) {
         pLevel++;
         GetToken(tok);
      }
         
      if (tok->token == TID) {
         strcpy(name, tok->str);
         if (pLevel == 0) {
            switch (type) {
            case TCHAR:
               printf("Local 'char' variable '%s'\n", name);
               autoSize += 2;
               break;
            case TINT:
               printf("Local 'int' variable '%s'\n", name);
               autoSize += 1;
               break;
            case TFLOAT:
               printf("Local 'float' variable '%s'\n", name);
               autoSize += 4;
               break;
            case TDOUBLE:
               printf("Local 'double' variable '%s'\n", name);
               autoSize += 8;
               break;
            }
         }
         else {
            printf("Local pointer variable '%s'\n", name);
               autoSize += 2;
         }
      }
      else {
         fprintf(stderr, "Expected identifier in local variable declaration\n");
      }
      
      GetToken(tok);
      
      if (tok->token != TSEMI) {
         fprintf(stderr, "Missing semicolon in local variable declaration\n");
      }

      GetToken(tok);
   }
   
   printf("Size of 'auto' variables: %d\n", autoSize);
   if (autoSize != 0) {
      char frame[32];
      
      snprintf(frame, sizeof (frame), "-%d,s", autoSize);
      Emit("leas", frame, "Allocate stack frame");
   }
   
   while (tok->token != TCBRACE) {
      ParseStatement(tok, returnLabel, NOLABEL, NOLABEL);
   }
   
   EmitLabel(returnLabel);
   Emit("tfr", "u,s", "Deallocate stack frame");
   Emit("puls", "u", "Restore frame pointer");
   Emit("rts", "", "Return to caller");

   printf("%s returns\n", __FUNCTION__);
}


/* ParseStatement --- parse a single statement */

void ParseStatement(struct Token *tok, const int returnLabel, const int breakLabel, const int continueLabel)
{
   switch (tok->token) {
   case TRETURN:
      ParseReturn(tok, returnLabel);
      break;
   case TIF:
      ParseIf(tok, returnLabel);
      break;
   case TDO:
      ParseDo(tok, returnLabel);
      break;
   case TFOR:
      printf("<for>\n");
      GetToken(tok);
      break;
   case TWHILE:
      printf("<while>\n");
      GetToken(tok);
      break;
   case TGOTO:
      ParseGoto(tok);
      break;
   case TCONTINUE:
      ParseContinue(tok, continueLabel);
      break;
   case TBREAK:
      ParseBreak(tok, breakLabel);
      break;
   case TSWITCH:
      printf("<switch>\n");
      GetToken(tok);
      break;
   case TOBRACE:
      ParseCompoundStatement(tok, returnLabel, breakLabel, continueLabel);
      break;
   default:
      ParseExpression(tok);
      break;
   }
   
   GetToken(tok);
}


/* ParseReturn --- parse a 'return' statement */

void ParseReturn(struct Token *tok, const int returnLabel)
{
   printf("<return>\n");
   GetToken(tok);

   if (tok->token != TSEMI) {
      ParseExpression(tok);
   }
   
   EmitJump(returnLabel, "Jump to return");

   if (tok->token != TSEMI) {
      fprintf(stderr, "Missing semicolon at end of 'return'\n");
   }
}


/* ParseCompoundStatement --- parse a compound statement */

void ParseCompoundStatement(struct Token *tok, const int returnLabel, const int breakLabel, const int continueLabel)
{
   printf("<compound_statement>\n");

   GetToken(tok);
   
   while (tok->token != TCBRACE) {
      ParseStatement(tok, returnLabel, breakLabel, continueLabel);
   }
}


/* ParseIf --- parse an 'if' statement */

void ParseIf(struct Token *tok, const int returnLabel)
{
   printf("<if>\n");
   GetToken(tok);
   
   if (tok->token != TSEMI) {
      fprintf(stderr, "Missing semicolon at end of 'if'\n");
   }
}


/* ParseExpression --- parse a single expression */

void ParseExpression(struct Token *tok)
{
   printf("<expression>\n");
   
   if (tok->token == TINTLIT) {
      LoadIntConstant(tok->iValue, 'D', tok->str);
   }
   else if (tok->token == TID) {
      LoadExternInt(tok->str, 'D', "Load int");
   }
   else {
      Emit("nop", "", "<expression>");
   }
   
   GetToken(tok);
}


/* ParseDo --- parse a 'do' statement */

void ParseDo(struct Token *tok, const int returnLabel)
{
   const int blabel = AllocLabel('b');
   const int clabel = AllocLabel('c');
   const int dlabel = AllocLabel('d');
   
   printf("<do>\n");
   EmitLabel(dlabel);

   GetToken(tok);
   ParseStatement(tok, returnLabel, blabel, clabel);

   if (tok->token != TWHILE) {
      fprintf(stderr, "Expected 'while' after 'do'\n");
   }
   else {
      GetToken(tok);
      
      if (tok->token == TOPAREN) {
         EmitLabel(clabel);
      
         GetToken(tok);
         ParseExpression(tok);
         
         Emit("cmpd", "#0", "do-while test");
         EmitBranchNotEqual(dlabel, "do-while branch");

         if (tok->token == TCPAREN) {
            GetToken(tok);
         }
         else {
            fprintf(stderr, "Expected ')' after 'while'\n");
         }
      }
      else {
         fprintf(stderr, "Expected '(' after 'while'\n");
      }
   }
   
   EmitLabel(blabel);
}


/* ParseBreak --- parse a 'break' statement and check that it's valid */

void ParseBreak(struct Token *tok, const int breakLabel)
{
   printf("<break>\n");
   
   if (breakLabel == NOLABEL) {
      fprintf(stderr, "'break' not inside a loop or 'switch'\n");
   }
   else {
      EmitJump(breakLabel, "break");
   }
   
   GetToken(tok);
   
   if (tok->token != TSEMI) {
      fprintf(stderr, "Expected ';' after 'break'\n");
   }
}


/* ParseContinue --- parse a 'continue' statement and check that it's valid */

void ParseContinue(struct Token *tok, const int continueLabel)
{
   printf("<continue>\n");
   
   if (continueLabel == NOLABEL) {
      fprintf(stderr, "'continue' not inside a loop\n");
   }
   else {
      EmitJump(continueLabel, "continue");
   }
   
   GetToken(tok);
   
   if (tok->token != TSEMI) {
      fprintf(stderr, "Expected ';' after 'continue'\n");
   }
}


/* ParseGoto --- parse and ignore a 'goto' statement */

void ParseGoto(struct Token *tok)
{
   printf("<goto>\n");
   GetToken(tok);
   
   if (tok->token == TID) {
      GetToken(tok);
      
      if (tok->token != TSEMI) {
         fprintf(stderr, "Expected ';' after 'goto'\n");
      }
   }
   else {
      fprintf(stderr, "Expected label name after 'goto'\n");
   }
}

