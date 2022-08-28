/* parser --- C parser                                      2022-07-31 */
/* Copyright (c) 2022 John Honniball. All rights reserved              */

#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#include "codegen.h"
#include "lexical.h"
#include "symtab.h"

#define MAXCASES (512)   // Maximum number of case labels in a 'switch'

void initialise(void);
void parse(const char fname[]);
void parser(const char fname[]);
int ParseDeclaration(struct Token *tok);
void ParseFunctionBody(struct Token *tok, const char name[], const int pLevel, const int type);
void ParseStatement(struct Token *tok, const int returnLabel, const int breakLabel, const int continueLabel);
void ParseReturn(struct Token *tok, const int returnLabel);
void ParseCompoundStatement(struct Token *tok, const int returnLabel, const int breakLabel, const int continueLabel);
void ParseIf(struct Token *tok, const int returnLabel, const int breakLabel, const int continueLabel);
void ParseExpression(struct Token *tok);
void ParseDo(struct Token *tok, const int returnLabel);
void ParseBreak(struct Token *tok, const int breakLabel);
void ParseContinue(struct Token *tok, const int continueLabel);
void ParseGoto(struct Token *tok);
void ParseWhile(struct Token *tok, const int returnLabel);
void ParseFor(struct Token *tok, const int returnLabel);
void ParseSwitch(struct Token *tok, const int returnLabel, const int continueLabel);
void ParseSemi(struct Token *tok, const char location[]);


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
   SymTabInit();
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
      GetToken(tok);
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

         AddExternSymbol(name, type, pLevel);
         
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
         ParseSemi(tok, "in declaration");
         break;
      case TOSQBRK:  // Array
         break;
      case TOPAREN:  // Function
         GetToken(tok);
         
         if (tok->token == TVOID) { // Only recognise 'void' functions as yet
            GetToken(tok);
         }
         
         if (tok->token == TCPAREN) {
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
         else {
            fprintf(stderr, "Malformed function declaration");
         }

         GetToken(tok);

         if (tok->token == TOBRACE) {
            printf("<function_definition>\n");
            ParseFunctionBody(tok, name, pLevel, type);
         }
         else {
            ParseSemi(tok, "in function declaration");
         }

         break;
      case TSEMI:    // Uninitialised scalar
         AddExternSymbol(name, type, pLevel);

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

   return (tok->token);
}


/* ParseFunctionBody --- parse the body of a function */

void ParseFunctionBody(struct Token *tok, const char name[], const int pLevel, const int type)
{
   int autoSize = 0;
   const int returnLabel = AllocLabel('R');
   
   GetToken(tok);
   
   // Function's local variables
   while ((tok->token == TINT) || (tok->token == TCHAR) ||
          (tok->token == TFLOAT) || (tok->token == TDOUBLE) ||
          (tok->token == TSTATIC) || (tok->token == TAUTO) ||
          (tok->token == TREGISTER)) {
      char name[256];
      int type;
      int pLevel = 0;
      bool isStatic = false;
      
      if (tok->token == TSTATIC) {           // Accept 'static' storage class
         GetToken(tok);
         isStatic = true;
      }
      else if (tok->token == TAUTO) {        // Ignore 'auto' storage class
         GetToken(tok);
      }
      else if (tok->token == TREGISTER) {    // Ignore 'register' storage class
         GetToken(tok);
      }
      
      type = tok->token;   // Yikes, we're assuming that the next token is a valid type!
      
      GetToken(tok);

      while (tok->token == TSTAR) {
         pLevel++;
         GetToken(tok);
      }
         
      if (tok->token == TID) {
         strcpy(name, tok->str);
         if (isStatic) {
            const int slabel = AllocLabel('S');
            
            if (pLevel == 0) {
               switch (type) {
               case TCHAR:
                  printf("Static 'char' variable '%s'\n", name);
                  EmitStaticChar(slabel, 0, name);
                  break;
               case TINT:
                  printf("Static 'int' variable '%s'\n", name);
                  EmitStaticInt(slabel, 0, name);
                  break;
               case TFLOAT:
                  printf("Static 'float' variable '%s'\n", name);
                  EmitStaticFloat(slabel, 0.0f, name);
                  break;
               case TDOUBLE:
                  printf("Static 'double' variable '%s'\n", name);
                  EmitStaticDouble(slabel, 0.0, name);
                  break;
               }
            }
            else {
               printf("Static pointer variable '%s'\n", name);
               EmitStaticInt(slabel, 0, name);
            }
         }
         else {
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
   
   // Function entry sequence
   EmitFunctionLabel(name);
   Emit("pshs", "u", "Save old frame pointer");
   Emit("tfr", "s,u", "Make new frame pointer");
   
   printf("Size of 'auto' variables: %d\n", autoSize);

   if (autoSize != 0) {
      char frame[32];
      
      snprintf(frame, sizeof (frame), "-%d,s", autoSize);
      Emit("leas", frame, "Allocate stack frame");
   }
   
   // Function's executable code
   while (tok->token != TCBRACE) {
      ParseStatement(tok, returnLabel, NOLABEL, NOLABEL);
   }
   
   GetToken(tok);
   
   // Function exit sequence
   EmitLabel(returnLabel);
   Emit("tfr", "u,s", "Deallocate stack frame");
   Emit("puls", "u", "Restore frame pointer");
   Emit("rts", "", "Return to caller");

   printf("%s returns\n", __FUNCTION__);
}


/* ParseStatement --- parse a single statement */

void ParseStatement(struct Token *tok, const int returnLabel, const int breakLabel, const int continueLabel)
{
   printf("<statement> ");
   
   switch (tok->token) {
   case TRETURN:
      ParseReturn(tok, returnLabel);
      break;
   case TIF:
      ParseIf(tok, returnLabel, breakLabel, continueLabel);
      break;
   case TDO:
      ParseDo(tok, returnLabel);
      break;
   case TFOR:
      ParseFor(tok, returnLabel);
      break;
   case TWHILE:
      ParseWhile(tok, returnLabel);
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
      ParseSwitch(tok, returnLabel, continueLabel);
      break;
   case TOBRACE:
      ParseCompoundStatement(tok, returnLabel, breakLabel, continueLabel);
      break;
   default:
      ParseExpression(tok);
      ParseSemi(tok, "after expression");
      break;
   }
}


/* ParseReturn --- parse a 'return' statement */

void ParseReturn(struct Token *tok, const int returnLabel)
{
   printf("<return> ");
   GetToken(tok);

   if (tok->token == TSEMI) {
      printf("\n");
   }
   else {
      ParseExpression(tok);
   }
   
   EmitJump(returnLabel, "Jump to return");

   ParseSemi(tok, "at end of 'return'");
}


/* ParseCompoundStatement --- parse a compound statement */

void ParseCompoundStatement(struct Token *tok, const int returnLabel, const int breakLabel, const int continueLabel)
{
   printf("<compound_statement>\n");

   GetToken(tok);
   
   while (tok->token != TCBRACE) {
      ParseStatement(tok, returnLabel, breakLabel, continueLabel);
   }

   GetToken(tok); /* Skip the closing curly bracket */
}


/* ParseIf --- parse an 'if' statement */

void ParseIf(struct Token *tok, const int returnLabel, const int breakLabel, const int continueLabel)
{
   const int elseLabel = AllocLabel('E');
   const int endifLabel = AllocLabel('I');
   
   printf("<if> ");
   GetToken(tok);
   
   if (tok->token == TOPAREN) {
      GetToken(tok);
      ParseExpression(tok);
      
      if (tok->token == TCPAREN) {
         Emit("cmpd", "#0", "if test");
         EmitBranchIfEqual(endifLabel, "if branch");
         GetToken(tok);
         ParseStatement(tok, returnLabel, breakLabel, continueLabel);
      }
      else {
         fprintf(stderr, "Expected ')' after <expression> in 'if'\n");
      }
   }
   else {
      fprintf(stderr, "Expected '(' after 'if'\n");
   }
   
   EmitLabel(endifLabel);
}


/* ParseExpression --- parse a single expression */

void ParseExpression(struct Token *tok)
{
   printf("<expression>");
   
   if (tok->token == TOPAREN) {
      printf("(");
      GetToken(tok);
      ParseExpression(tok);
      if (tok->token == TCPAREN) {
         GetToken(tok);
         printf(")");
      }
      else {
         fprintf(stderr, "Expected ')' after expression\n");
      }
   }
   else if (tok->token == TINTLIT) {
      LoadIntConstant(tok->iValue, 'D', tok->str);
      GetToken(tok);
   }
   else if (tok->token == TID) {
      char name[256];
      
      strncpy(name, tok->str, sizeof (name));
      
      // TODO: look up ID in symbol table

      LoadExternInt(name, 'D', "Load int");
      GetToken(tok);

      if ((tok->token == TINC) || (tok->token == TDEC)) {
         if (tok->token == TINC) {
            EmitIncExternInt(name, 1);
         }
         else if (tok->token == TDEC) {
            EmitIncExternInt(name, -1);
         }
         GetToken(tok);
      }
   }
   else {
      Emit("nop", "", "<expression>");
      GetToken(tok);
   }
   
   
   printf("\n");
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
      printf("<while> ");
      GetToken(tok);
      
      if (tok->token == TOPAREN) {
         EmitLabel(clabel);
      
         GetToken(tok);
         ParseExpression(tok);
         
         Emit("cmpd", "#0", "do-while test");
         EmitBranchNotEqual(dlabel, "do-while branch");

         if (tok->token == TCPAREN) {
            GetToken(tok);
            ParseSemi(tok, "after 'do'-'while'");
         }
         else {
            fprintf(stderr, "Expected ')' after 'do'-'while'\n");
         }
      }
      else {
         fprintf(stderr, "Expected '(' after 'do'-'while'\n");
      }
   }
   
   EmitLabel(blabel);
}


/* ParseBreak --- parse a 'break' statement and check that it's valid */

void ParseBreak(struct Token *tok, const int breakLabel)
{
   printf("<break>\n");
   GetToken(tok);
   
   if (breakLabel == NOLABEL) {
      fprintf(stderr, "'break' not inside a loop or 'switch'\n");
   }
   else {
      EmitJump(breakLabel, "break");
   }
   
   ParseSemi(tok, "after 'break'");
}


/* ParseContinue --- parse a 'continue' statement and check that it's valid */

void ParseContinue(struct Token *tok, const int continueLabel)
{
   printf("<continue>\n");
   GetToken(tok);
   
   if (continueLabel == NOLABEL) {
      fprintf(stderr, "'continue' not inside a loop\n");
   }
   else {
      EmitJump(continueLabel, "continue");
   }
   
   ParseSemi(tok, "after 'continue'");
}


/* ParseGoto --- parse and ignore a 'goto' statement */

void ParseGoto(struct Token *tok)
{
   printf("<goto>\n");
   GetToken(tok);
   
   if (tok->token == TID) {
      GetToken(tok); /* Ignore target label */
      
      ParseSemi(tok, "after 'goto'");
   }
   else {
      fprintf(stderr, "Expected label name after 'goto'\n");
   }
}


/* ParseWhile --- parse a 'while' statement */

void ParseWhile(struct Token *tok, const int returnLabel)
{
   const int blabel = AllocLabel('b');
   const int clabel = AllocLabel('c');
   
   printf("<while> ");
   GetToken(tok);
   
   if (tok->token == TOPAREN) {
      EmitLabel(clabel);

      GetToken(tok);
      
      ParseExpression(tok);
      
      Emit("cmpd", "#0", "while test");
      EmitBranchIfEqual(blabel, "while branch");

      if (tok->token == TCPAREN) {
         GetToken(tok);
         
         ParseStatement(tok, returnLabel, blabel, clabel);
         EmitJump(clabel, "while loop");
      }
      else {
         fprintf(stderr, "Expected ')' after 'while'\n");
      }
   }
   else {
      fprintf(stderr, "Expected '(' after 'while'\n");
   }

   EmitLabel(blabel);
}


/* ParseFor --- parse a 'for' statement */

void ParseFor(struct Token *tok, const int returnLabel)
{
   printf("<for>\n");
   GetToken(tok);
   ParseSemi(tok, "after 'for'");
}


/* ParseSwitch --- parse a 'switch' statement */

void ParseSwitch(struct Token *tok, const int returnLabel, const int continueLabel)
{
   const int blabel = AllocLabel('b');
   const int jlabel = AllocLabel('J'); // Label for jump over labelled_compound_statement to compare/branch chain
   int dlabel = blabel;    // Default target for 'default' label is same as 'break'
   int clabel;             // Label for each case
   int nCases = 0;
   int i;
   struct {
      int match;
      int label;
   } cases[MAXCASES];

   printf("<switch> ");
   GetToken(tok);

   if (tok->token == TOPAREN) {
      GetToken(tok);
      
      ParseExpression(tok);
      
      EmitJump(jlabel, "switch: jump to compares");
         
      if (tok->token == TCPAREN) {
         GetToken(tok);

         printf("<labelled_compound_statement>\n");

         if (tok->token == TOBRACE) {
            GetToken(tok);
            
            while (tok->token != TCBRACE) {
               if (tok->token == TCASE) {
                  GetToken(tok);

                  if (tok->token == TINTLIT) {
                     printf("<case> %d: ", tok->iValue);
                     cases[nCases].match = tok->iValue;
                     GetToken(tok);

                     if (tok->token == TCOLON) {
                        GetToken(tok);
                        clabel = AllocLabel('C');  // TODO: check case numbers are unique
                        cases[nCases].label = clabel;
                        nCases++;
                        EmitLabel(clabel);
                     }
                     else {
                        fprintf(stderr, "Expected ':' after 'case'\n");
                     }
                  }
                  else {
                     fprintf(stderr, "Expected integer constant after 'case'\n");
                  }
               }
               else if (tok->token == TDEFAULT) {
                  printf("<default> ");
                  GetToken(tok);
                  
                  if (tok->token == TCOLON) {
                     if (dlabel == blabel) {
                        dlabel = AllocLabel('D');
                        EmitLabel(dlabel);
                     }
                     else {
                        fprintf(stderr, "Multiple 'default:' labels in 'switch'\n");
                     }
                     
                     GetToken(tok);
                  }
                  else {
                     fprintf(stderr, "Expected ':' after 'default'\n");
                  }
               }
               else {
                  ParseStatement(tok, returnLabel, blabel, continueLabel);
               }
            }

            GetToken(tok); /* Skip the closing curly bracket */
         }
         else {
            fprintf(stderr, "Expected '{' after 'switch'\n");
         }
      }
      else {
         fprintf(stderr, "Expected ')' after 'switch'\n");
      }
   }
   else {
      fprintf(stderr, "Expected '(' after 'switch'\n");
   }

   EmitJump(blabel, "switch jump over compares");

   EmitLabel(jlabel);

   for (i = 0; i < nCases; i++) {
      char caseMatch[16];
      
      snprintf(caseMatch, sizeof (caseMatch), "#%d", cases[i].match);
      
      Emit("cmpd", caseMatch, "switch compare");
      EmitBranchIfEqual(cases[i].label, "branch back to code");
   }
   
   if (dlabel != blabel) {
      EmitJump(dlabel, "switch jump to default");
   }
   
   EmitLabel(blabel);
}


/* ParseSemi --- parse a semicolon */

void ParseSemi(struct Token *tok, const char location[])
{
   if (tok->token == TSEMI) {
      GetToken(tok);
   }
   else {
      fprintf(stderr, "Missing semicolon %s\n", location);
   }
}
