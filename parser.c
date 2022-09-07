/* parser --- C parser                                      2022-07-31 */
/* Copyright (c) 2022 John Honniball. All rights reserved              */

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "codegen.h"
#include "lexical.h"
#include "symtab.h"

//#define LEX_TESTER

#define MAXCASES (512)   // Maximum number of case labels in a 'switch'

struct StringConstant Strings[64];
static int NextStr = 0;


void initialise(void);
void parse(const char fname[]);
void parser(void);
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
bool ParseConstIntExpr(struct Token *tok, int *value, int *type);


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
         case 'S':
            SetSyntaxTraceFlag(true);
            break;
         default:
            fprintf(stderr, "Usage: %s [-T] [-S] <filename>\n", argv[0]);
            exit(EXIT_FAILURE);
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
   
   parser();

   CloseAssemblerFile();
   CloseSourceFile();
}


/* parser --- parse and translate a single compilation-unit */

void parser(void)
{
   struct Token tok;

#ifdef LEX_TESTER
   while (GetToken(&tok) != TEOF)
      PrintToken(&tok);
#else
   GetToken(&tok);

   while (ParseDeclaration(&tok) != EOF)
      ;
#endif
}


/* ParseDeclaration --- parse a top-level declaration */

int ParseDeclaration(struct Token *tok)
{
   char name[256];
   int type;
   int pLevel;
   int iValue;
   int iType;
   
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
         Error("Missing identifier in declaration");
      }

      switch (tok->token) {
      case TASSIGN:  // Scalar initialiser
         GetToken(tok);

         AddExternSymbol(name, type, pLevel);
         
         if (pLevel == 0) {
            switch (type) {
            case TCHAR:
               if (ParseConstIntExpr(tok, &iValue, &iType)) {
                  printf("Initialised char '%s' %d\n", name, iValue);
                  EmitExternChar(name, iValue, "char");
               }
               else {
                  Error("Expected integer constant after '='");
               }
               break;
            case TINT:
               if (ParseConstIntExpr(tok, &iValue, &iType)) {
                  printf("Initialised int '%s' %d\n", name, iValue);
                  EmitExternInt(name, iValue, "int");
               }
               else {
                  Error("Expected integer constant after '='");
               }
               break;
            case TFLOAT:
               printf("Initialised float '%s' '%s'\n", name, tok->str);
               EmitExternFloat(name, tok->fValue, "float");
               GetToken(tok);
               break;
            case TDOUBLE:
               printf("Initialised double '%s' '%s'\n", name, tok->str);
               EmitExternDouble(name, tok->fValue, "double");
               GetToken(tok);
               break;
            }
         }
         else {
            printf("Initialised pointer '%s' '%s'\n", name, tok->str);
            EmitExternPointer(name, tok->iValue, "pointer");
            GetToken(tok);
         }
         
         ParseSemi(tok, "in declaration");
         break;
      case TOSQBRK:  // Array
         GetToken(tok);
         if (ParseConstIntExpr(tok, &iValue, &iType)) {
            printf("Array %d\n", iValue);
            if (tok->token == TCSQBRK) {
               GetToken(tok);
            }
            else {
               Error("Expected ']' after array dimension");
            }
         }
         else {
            Error("Expected integer constant after '['");
         }
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
            Error("Malformed function declaration");
         }

         GetToken(tok);

         if (tok->token == TOBRACE) {
            PrintSyntax("<function_definition>\n");
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
         Error("Unexpected symbol '%s' in declaration", tok->str);
         break;
      }
      
      break;
   default:
      Error("Unexpected symbol '%s' in declaration", tok->str);
      break;
   }

   return (tok->token);
}


/* ParseFunctionBody --- parse the body of a function */

void ParseFunctionBody(struct Token *tok, const char name[], const int pLevel, const int type)
{
   int autoSize = 0;
   int i;
   const int returnLabel = AllocLabel('R');
   
   NextStr = 0;

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
         Error("Expected identifier in local variable declaration");
      }
      
      GetToken(tok);
      
      ParseSemi(tok, "in local variable declaration");
   }
   
   // Function entry sequence
   printf("Size of 'auto' variables: %d\n", autoSize);
   EmitFunctionEntry(name, autoSize);

   // Function's executable code
   while (tok->token != TCBRACE) {
      ParseStatement(tok, returnLabel, NOLABEL, NOLABEL);
   }
   
   GetToken(tok);
   
   // Function exit sequence
   EmitFunctionExit(returnLabel);
   
   for (i = 0; i < NextStr; i++) {
      EmitStaticCharArray(&Strings[i], "<anon>");
   }
   
   NextStr = 0;
}


/* ParseStatement --- parse a single statement */

void ParseStatement(struct Token *tok, const int returnLabel, const int breakLabel, const int continueLabel)
{
   PrintSyntax("<statement> ");
   
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
   PrintSyntax("<return> ");
   GetToken(tok);

   if (tok->token == TSEMI) {
      PrintSyntax("\n");
   }
   else {
      ParseExpression(tok);
   }
   
   EmitJump(returnLabel, "return");

   ParseSemi(tok, "at end of 'return'");
}


/* ParseCompoundStatement --- parse a compound statement */

void ParseCompoundStatement(struct Token *tok, const int returnLabel, const int breakLabel, const int continueLabel)
{
   PrintSyntax("<compound_statement>\n");

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
   
   PrintSyntax("<if> ");
   GetToken(tok);
   
   if (tok->token == TOPAREN) {
      GetToken(tok);
      ParseExpression(tok);
      
      if (tok->token == TCPAREN) {
         EmitCompareIntConstant(0, "if: test");
         EmitBranchIfEqual(elseLabel, "if: branch");
         GetToken(tok);
         ParseStatement(tok, returnLabel, breakLabel, continueLabel);
         
         if (tok->token == TELSE) {
            const int endifLabel = AllocLabel('I');

            GetToken(tok);
            EmitJump(endifLabel, "if: jump to endif");
            EmitLabel(elseLabel);
            ParseStatement(tok, returnLabel, breakLabel, continueLabel);
            EmitLabel(endifLabel);
         }
         else {
            EmitLabel(elseLabel);
         }
      }
      else {
         Error("Expected ')' after <expression> in 'if'");
      }
   }
   else {
      Error("Expected '(' after 'if'");
   }
}


/* ParseExpression --- parse a single expression */

void ParseExpression(struct Token *tok)
{
   PrintSyntax("<expression>");
   
   if (tok->token == TOPAREN) {
      PrintSyntax("(");
      GetToken(tok);
      ParseExpression(tok);
      if (tok->token == TCPAREN) {
         GetToken(tok);
         PrintSyntax(")");
      }
      else {
         Error("Expected ')' after expression");
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

      GetToken(tok);

      if ((tok->token == TINC) || (tok->token == TDEC)) {
         LoadExternInt(name, 'D', "Load int");

         if (tok->token == TINC) {
            EmitIncExternInt(name, 1);
         }
         else if (tok->token == TDEC) {
            EmitIncExternInt(name, -1);
         }

         GetToken(tok);
      }
      else if (tok->token == TOPAREN) {
         GetToken(tok);

         // Parse arguments here

         if (tok->token == TCPAREN) {
            EmitCallFunction(name, "call function");
            GetToken(tok);
         }
         else {
            Error("Expected ')' in function call");
         }
      }
      else {
         LoadExternInt(name, 'D', "Load int");
      }
   }
   else if (tok->token == TSTRLIT) {
      const int strLit = AllocLabel('S');
      
      Strings[NextStr].label = strLit;
      strncpy(Strings[NextStr].str, tok->str, 256);
      memcpy(Strings[NextStr].sValue, tok->sValue, tok->sLength);
      Strings[NextStr].sLength = tok->sLength;
      NextStr++;
      
      LoadLabelAddr(strLit, tok->str);
      GetToken(tok);
   }
   else {
      Emit("nop", "", "<expression>");
      GetToken(tok);
   }
   
   PrintSyntax("\n");
}


/* ParseDo --- parse a 'do' statement */

void ParseDo(struct Token *tok, const int returnLabel)
{
   const int blabel = AllocLabel('b');
   const int clabel = AllocLabel('c');
   const int dlabel = AllocLabel('d');
   
   PrintSyntax("<do>\n");
   EmitLabel(dlabel);

   GetToken(tok);
   ParseStatement(tok, returnLabel, blabel, clabel);

   if (tok->token != TWHILE) {
      Error("Expected 'while' after 'do'");
   }
   else {
      PrintSyntax("<while> ");
      GetToken(tok);
      
      if (tok->token == TOPAREN) {
         EmitLabel(clabel);
      
         GetToken(tok);
         ParseExpression(tok);
         
         EmitCompareIntConstant(0, "do-while: test");
         EmitBranchNotEqual(dlabel, "do-while: branch");

         if (tok->token == TCPAREN) {
            GetToken(tok);
            ParseSemi(tok, "after 'do'-'while'");
         }
         else {
            Error("Expected ')' after 'do'-'while'");
         }
      }
      else {
         Error("Expected '(' after 'do'-'while'");
      }
   }
   
   EmitLabel(blabel);
}


/* ParseBreak --- parse a 'break' statement and check that it's valid */

void ParseBreak(struct Token *tok, const int breakLabel)
{
   PrintSyntax("<break>\n");
   GetToken(tok);
   
   if (breakLabel == NOLABEL) {
      Error("'break' not inside a loop or 'switch'");
   }
   else {
      EmitJump(breakLabel, "break");
   }
   
   ParseSemi(tok, "after 'break'");
}


/* ParseContinue --- parse a 'continue' statement and check that it's valid */

void ParseContinue(struct Token *tok, const int continueLabel)
{
   PrintSyntax("<continue>\n");
   GetToken(tok);
   
   if (continueLabel == NOLABEL) {
      Error("'continue' not inside a loop");
   }
   else {
      EmitJump(continueLabel, "continue");
   }
   
   ParseSemi(tok, "after 'continue'");
}


/* ParseGoto --- parse and ignore a 'goto' statement */

void ParseGoto(struct Token *tok)
{
   PrintSyntax("<goto>\n");
   GetToken(tok);
   
   if (tok->token == TID) {
      GetToken(tok); /* Ignore target label */
      
      ParseSemi(tok, "after 'goto'");
   }
   else {
      Error("Expected label name after 'goto'");
   }
}


/* ParseWhile --- parse a 'while' statement */

void ParseWhile(struct Token *tok, const int returnLabel)
{
   const int blabel = AllocLabel('b');
   const int clabel = AllocLabel('c');
   
   PrintSyntax("<while> ");
   GetToken(tok);
   
   if (tok->token == TOPAREN) {
      EmitLabel(clabel);

      GetToken(tok);
      
      ParseExpression(tok);
      
      EmitCompareIntConstant(0, "while: test");
      EmitBranchIfEqual(blabel, "while: exit");

      if (tok->token == TCPAREN) {
         GetToken(tok);
         
         ParseStatement(tok, returnLabel, blabel, clabel);
         EmitJump(clabel, "while: loop");
      }
      else {
         Error("Expected ')' after 'while'");
      }
   }
   else {
      Error("Expected '(' after 'while'");
   }

   EmitLabel(blabel);
}


/* ParseFor --- parse a 'for' statement */

void ParseFor(struct Token *tok, const int returnLabel)
{
   const int blabel = AllocLabel('b');
   const int clabel = AllocLabel('c');
   const int slabel = AllocLabel('s');
   const int tlabel = AllocLabel('t');

   PrintSyntax("<for> ");
   GetToken(tok);

   if (tok->token == TOPAREN) {
      GetToken(tok);
      
      ParseExpression(tok);      // Initialisation
      
      ParseSemi(tok, "in 'for'");
      
      EmitLabel(tlabel);

      ParseExpression(tok);      // Test

      EmitCompareIntConstant(0, "for: test");
      EmitBranchIfEqual(blabel, "for: exit");
      EmitJump(slabel, "for: jump to statement");

      ParseSemi(tok, "in 'for'");

      EmitLabel(clabel);

      ParseExpression(tok);      // Increment

      EmitJump(tlabel, "for: jump back to test");

      if (tok->token == TCPAREN) {
         GetToken(tok);
         
         EmitLabel(slabel);
         ParseStatement(tok, returnLabel, blabel, clabel);
         EmitJump(clabel, "for: loop");
      }
      else {
         Error("Expected ')' after 'for'");
      }
   }
   else {
      Error("Expected '(' after 'for'");
   }

   EmitLabel(blabel);
}


/* ParseSwitch --- parse a 'switch' statement */

void ParseSwitch(struct Token *tok, const int returnLabel, const int continueLabel)
{
   const int blabel = AllocLabel('b');
   const int jlabel = AllocLabel('J'); // Label for jump over labelled_compound_statement to compare/branch chain
   int dlabel = blabel;    // Default target for 'default' label is same as 'break'
   int clabel;             // Label for each case
   int iValue = 0;
   int iType = 0;
   int nCases = 0;
   int i;
   struct {
      int match;
      int label;
   } cases[MAXCASES];

   PrintSyntax("<switch> ");
   GetToken(tok);

   if (tok->token == TOPAREN) {
      GetToken(tok);
      
      ParseExpression(tok);
      
      EmitJump(jlabel, "switch: jump to compares");
         
      if (tok->token == TCPAREN) {
         GetToken(tok);

         PrintSyntax("<labelled_compound_statement>\n");

         if (tok->token == TOBRACE) {
            GetToken(tok);
            
            while (tok->token != TCBRACE) {
               if (tok->token == TCASE) {
                  GetToken(tok);

                  if (ParseConstIntExpr(tok, &iValue, &iType)) {
                     PrintSyntax("<case> %d: ", iValue);
                     cases[nCases].match = iValue;

                     if (tok->token == TCOLON) {
                        GetToken(tok);
                        clabel = AllocLabel('C');  // TODO: check case numbers are unique
                        cases[nCases].label = clabel;
                        nCases++;
                        EmitLabel(clabel);
                     }
                     else {
                        Error("Expected ':' after 'case'");
                     }
                  }
                  else {
                     Error("Expected integer constant after 'case'");
                  }
               }
               else if (tok->token == TDEFAULT) {
                  PrintSyntax("<default> ");
                  GetToken(tok);
                  
                  if (tok->token == TCOLON) {
                     if (dlabel == blabel) {
                        dlabel = AllocLabel('D');
                        EmitLabel(dlabel);
                     }
                     else {
                        Error("Multiple 'default:' labels in 'switch'");
                     }
                     
                     GetToken(tok);
                  }
                  else {
                     Error("Expected ':' after 'default'");
                  }
               }
               else {
                  ParseStatement(tok, returnLabel, blabel, continueLabel);
               }
            }

            GetToken(tok); /* Skip the closing curly bracket */
         }
         else {
            Error("Expected '{' after 'switch'");
         }
      }
      else {
         Error("Expected ')' after 'switch'");
      }
   }
   else {
      Error("Expected '(' after 'switch'");
   }

   EmitJump(blabel, "switch: jump over compares");

   EmitLabel(jlabel);

   for (i = 0; i < nCases; i++) {
      EmitCompareIntConstant(cases[i].match, "switch: compare");
      EmitBranchIfEqual(cases[i].label, "switch: branch to code");
   }
   
   if (dlabel != blabel) {
      EmitJump(dlabel, "switch: default");
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
      Error("Missing semicolon %s", location);
   }
}


/* ParseConstIntExpr --- parse a constant integer expression, e.g. after a 'case' */

bool ParseConstIntExpr(struct Token *tok, int *value, int *type)
{
   bool ret = true;
   
   if (tok->token == TOPAREN) {
      GetToken(tok);
      ret = ParseConstIntExpr(tok, value, type);
      if (tok->token == TCPAREN) {
         GetToken(tok);
      }
      else {
         Error("Expected ')' in constant expression");
         ret = false;
      }
   }
   else if (tok->token == TINTLIT) {
      *value = tok->iValue;
      *type = TINT;
      GetToken(tok);
   }
   else {
      ret = false;
   }
      
   if ((tok->token == TPLUS) || (tok->token == TMINUS) ||
       (tok->token == TSTAR) || (tok->token == TDIV) ||
       (tok->token == TMOD)) {
      const int op = tok->token;
      int rhsValue;
      int rhsType;
      
      GetToken(tok);
      ret = ParseConstIntExpr(tok, &rhsValue, &rhsType);
      switch (op) {
      case TPLUS:
         *value += rhsValue;
         break;
      case TMINUS:
         *value -= rhsValue;
         break;
      case TSTAR: 
         *value *= rhsValue;
         break;
      case TDIV:
         *value /= rhsValue;
         break;
      case TMOD:
         *value %= rhsValue;
         break;
      }
   }
   
   return (ret);
}

