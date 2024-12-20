/* parser --- C parser                                      2022-07-31 */
/* Copyright (c) 2022 John Honniball. All rights reserved              */

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "codegen.h"
#include "lexical.h"

//#define LEX_TESTER

#define MAXCASES (512)   // Maximum number of case labels in a 'switch'

struct StringConstant Strings[64];
static int NextStr = 0;


void initialise(void);
void parse(const char fname[]);
void parser(void);
int ParseDeclaration(struct Token *tok);
void ParseFunctionBody(struct Token *tok, const struct Symbol *const fn);
void ParseStatement(struct Token *tok, const struct Symbol *const fn, const int returnLabel, const int breakLabel, const int continueLabel);
void ParseReturn(struct Token *tok, const struct Symbol *const fn, const int returnLabel);
void ParseCompoundStatement(struct Token *tok, const struct Symbol *const fn, const int returnLabel, const int breakLabel, const int continueLabel);
void ParseIf(struct Token *tok, const struct Symbol *const fn, const int returnLabel, const int breakLabel, const int continueLabel);
void ParseExpression(struct Token *tok);
void ParseDo(struct Token *tok, const struct Symbol *const fn, const int returnLabel);
void ParseBreak(struct Token *tok, const int breakLabel);
void ParseContinue(struct Token *tok, const int continueLabel);
void ParseGoto(struct Token *tok);
void ParseWhile(struct Token *tok, const struct Symbol *const fn, const int returnLabel);
void ParseFor(struct Token *tok, const struct Symbol *const fn, const int returnLabel);
void ParseSwitch(struct Token *tok, const struct Symbol *const fn, const int returnLabel, const int continueLabel);
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
   struct Symbol sym;
   int type;
   int iValue;
   int iType;
   int paramSize = 0;
   
   type = 0;
   
   sym.name[0] = '\0';
   sym.storageClass = SCEXTERN;
   sym.type = T_INT;
   sym.pLevel = 0;
   sym.label = NOLABEL;
   sym.fpOffset = 0;
   sym.readOnly = false;
   
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
      PrintSyntax("<type>");
      type = tok->token;

      GetToken(tok);

      while (tok->token == TSTAR) {
         PrintSyntax("<'*'>");
         sym.pLevel++;
         GetToken(tok);
      }
      
      if (tok->token == TID) {
         PrintSyntax("<id:%s>", tok->str);
         strncpy(sym.name, tok->str, MAXNAME);
         GetToken(tok);
      }
      else {
         Error("Missing identifier in declaration");
      }

      switch (tok->token) {
      case TASSIGN:  // Scalar initialiser
         PrintSyntax("<'='>");
         GetToken(tok);

         if (sym.pLevel == 0) {
            switch (type) {
            case TCHAR:
               sym.type = T_CHAR;
               if (ParseConstIntExpr(tok, &iValue, &iType)) {
                  EmitExternScalar(&sym, iValue, 0.0);
               }
               else {
                  Error("Expected integer constant after '='");
               }
               break;
            case TINT:
               sym.type = T_INT;
               if (ParseConstIntExpr(tok, &iValue, &iType)) {
                  EmitExternScalar(&sym, iValue, 0.0);
               }
               else {
                  Error("Expected integer constant after '='");
               }
               break;
            case TFLOAT:
               sym.type = T_FLOAT;
               EmitExternScalar(&sym, 0, tok->fValue);
               GetToken(tok);
               break;
            case TDOUBLE:
               sym.type = T_DOUBLE;
               EmitExternScalar(&sym, 0, tok->fValue);
               GetToken(tok);
               break;
            }
         }
         else {
            EmitExternScalar(&sym, tok->iValue, 0.0);
            GetToken(tok);
         }
         
         if (AddExternSymbol(&sym) == false) {
            Error("Symbol '%s' is already declared", sym.name);
         }
         
         ParseSemi(tok, "in declaration");
         break;
      case TOSQBRK:  // Array
         PrintSyntax("[");
         GetToken(tok);
         if (ParseConstIntExpr(tok, &iValue, &iType)) {
            if (tok->token == TCSQBRK) {
               PrintSyntax("]");
               GetToken(tok);
            }
            else {
               Error("Expected ']' after array dimension");
            }
         }
         else {
            Error("Expected integer constant after '['");
         }

         ParseSemi(tok, "in array declaration");
         break;
      case TOPAREN:  // Function
         PrintSyntax("(");
         GetToken(tok);
         paramSize = 0;
         
         // Function's formal parameters
         while ((tok->token == TINT) || (tok->token == TCHAR) ||
                (tok->token == TFLOAT) || (tok->token == TDOUBLE) ||
                (tok->token == TVOID) || (tok->token == TCONST)) {
            struct Symbol param;
            int type;
            
            PrintSyntax("<formal_parameter>");
            
            param.storageClass = SCAUTO;
            param.name[0] = '\0';
            param.type = T_INT;
            param.pLevel = 0;
            param.label = NOLABEL;
            param.fpOffset = 0;
            param.readOnly = false;
            
            if (tok->token == TCONST) {
               GetToken(tok);
               
               param.readOnly = true;
            }
            
            type = tok->token;   // Yikes, we're assuming that the next token is a valid type!
            
            GetToken(tok);
            
            if (type == TVOID) { // 'void' must be alone with no identifier following
               break;
            }
            
            while (tok->token == TSTAR) {
               PrintSyntax("<'*'>");
               param.pLevel++;
               GetToken(tok);
            }
            
            if (tok->token == TID) {
               PrintSyntax("<id:%s>", tok->str);
               strncpy(param.name, tok->str, MAXNAME);

               if (param.pLevel == 0) {
                  switch (type) {
                  case TCHAR:
                     param.type = T_CHAR;
                     paramSize += 2;
                     break;
                  case TINT:
                     param.type = T_INT;
                     paramSize += 2;
                     break;
                  case TFLOAT:
                     param.type = T_FLOAT;
                     paramSize += 4;
                     break;
                  case TDOUBLE:
                     param.type = T_DOUBLE;
                     paramSize += 8;
                     break;
                  }
               }
               else {
                  param.type = T_INT;
                  paramSize += 2;
               }

               param.fpOffset = paramSize;
            }
            else {
               Error("Expected identifier in parameter declaration");
            }
            
            AddLocalSymbol(&param);
            
            PrintSyntax("\n");
            GetToken(tok);
            
            if (tok->token == TCOMMA) {
               GetToken(tok);
            }
            else if (tok->token != TCPAREN) {
               Error("Missing comma in parameter list");
            }
         }
         
         if (tok->token == TCPAREN) {
            PrintSyntax(")");

            if (sym.pLevel == 0) {
               switch (type) {
               case TVOID:
                  sym.type = T_VOID;
                  break;
               case TCHAR:
                  sym.type = T_INT;
                  break;
               case TINT:
                  sym.type = T_INT;
                  break;
               case TFLOAT:
                  sym.type = T_FLOAT;
                  break;
               case TDOUBLE:
                  sym.type = T_DOUBLE;
                  break;
               }
            }
            else {
               sym.type = T_INT;
            }
         }
         else {
            Error("Malformed function declaration");
         }

         GetToken(tok);

         sym.readOnly = true;
         
         if (AddExternSymbol(&sym) == false) {
            // Functions may already have prototypes or K&R-style declarations
            //Error("Symbol '%s' is already declared", sym.name);
         }

         if (tok->token == TOBRACE) {
            PrintSyntax("<function_definition>\n");
            ParseFunctionBody(tok, &sym);
         }
         else {
            PrintSyntax("<function_prototype>");
            ParseSemi(tok, "in function prototype");
         }
         
         sym.readOnly = false;

         break;
      case TSEMI:    // Uninitialised scalar
         GetToken(tok);
         
         if (sym.pLevel == 0) {
            switch (type) {
            case TCHAR:
               sym.type = T_CHAR;
               EmitExternScalar(&sym, 0, 0.0);
               break;
            case TINT:
               sym.type = T_INT;
               EmitExternScalar(&sym, 0, 0.0);
               break;
            case TFLOAT:
               sym.type = T_FLOAT;
               EmitExternScalar(&sym, 0, 0.0);
               break;
            case TDOUBLE:
               sym.type = T_DOUBLE;
               EmitExternScalar(&sym, 0, 0.0);
               break;
            }
         }
         else {
            sym.type = T_INT;
            EmitExternScalar(&sym, 0, 0.0);
         }

         if (AddExternSymbol(&sym) == false) {
            Error("Symbol '%s' is already declared", sym.name);
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

   PrintSyntax("\n");
   
   return (tok->token);
}


/* ParseFunctionBody --- parse the body of a function */

void ParseFunctionBody(struct Token *tok, const struct Symbol *const fn)
{
   int autoSize = 0;
   int nRegister = 0;
   int i;
   const int returnLabel = AllocLabel('R');
   
   NextStr = 0;

   GetToken(tok);
   
   // Function's local variables
   while ((tok->token == TINT) || (tok->token == TCHAR) ||
          (tok->token == TFLOAT) || (tok->token == TDOUBLE) ||
          (tok->token == TSTATIC) || (tok->token == TAUTO) ||
          (tok->token == TREGISTER) ||(tok->token == TCONST)) {
      struct Symbol sym;
      int type;
      bool isRegister = false;
      
      PrintSyntax("<declaration>");
      
      sym.storageClass = SCAUTO;
      sym.name[0] = '\0';
      sym.type = T_INT;
      sym.pLevel = 0;
      sym.label = NOLABEL;
      sym.fpOffset = 0;
      sym.readOnly = false;
      
      if (tok->token == TSTATIC) {           // Accept 'static' storage class
         PrintSyntax("<static>");
         GetToken(tok);
         sym.storageClass = SCSTATIC;
      }
      else if (tok->token == TCONST) {       // Accept 'const' attribute
         PrintSyntax("<const>");
         GetToken(tok);
         sym.readOnly = true;
      }
      else if (tok->token == TAUTO) {        // Ignore 'auto' storage class
         PrintSyntax("<auto>");
         GetToken(tok);
      }
      else if (tok->token == TREGISTER) {    // Accept 'register' storage class
         PrintSyntax("<register>");
         GetToken(tok);
         isRegister = true;
      }
      
      type = tok->token;   // Yikes, we're assuming that the next token is a valid type!
      
      GetToken(tok);

      while (tok->token == TSTAR) {
         PrintSyntax("<'*'>");
         sym.pLevel++;
         GetToken(tok);
      }
         
      // Decide whether to accept the 'register' storage class
      if (isRegister && (sym.storageClass == SCAUTO)) {
         if (((type == TCHAR) || (type == TINT)) && (nRegister < 1)) {
            sym.storageClass = SCREGISTER;
            nRegister++;
         }
      }
      
      if (tok->token == TID) {
         PrintSyntax("<id:%s>", tok->str);
         strncpy(sym.name, tok->str, MAXNAME);

         if (sym.storageClass == SCSTATIC) {
            sym.label = AllocLabel('S');
            
            if (sym.pLevel == 0) {
               switch (type) {
               case TCHAR:
                  sym.type = T_CHAR;
                  break;
               case TINT:
                  sym.type = T_INT;
                  break;
               case TFLOAT:
                  sym.type = T_FLOAT;
                  break;
               case TDOUBLE:
                  sym.type = T_DOUBLE;
                  break;
               }
            }
            else {
               sym.type = T_INT;
            }
         
            EmitExternScalar(&sym, 0, 0.0);
         }
         else if (sym.storageClass == SCREGISTER) {
            if (sym.pLevel == 0) {
               switch (type) {
               case TCHAR:
                  sym.type = T_CHAR;
                  break;
               case TINT:
                  sym.type = T_INT;
                  break;
               case TFLOAT:
                  sym.type = T_FLOAT;
                  break;
               case TDOUBLE:
                  sym.type = T_DOUBLE;
                  break;
               }
            }
            else {
               sym.type = T_INT;
            }
         }
         else {
            if (sym.pLevel == 0) {
               switch (type) {
               case TCHAR:
                  sym.type = T_CHAR;
                  autoSize += 1;
                  break;
               case TINT:
                  sym.type = T_INT;
                  autoSize += 2;
                  break;
               case TFLOAT:
                  sym.type = T_FLOAT;
                  autoSize += 4;
                  break;
               case TDOUBLE:
                  sym.type = T_DOUBLE;
                  autoSize += 8;
                  break;
               }
            }
            else {
               sym.type = T_INT;
               autoSize += 2;
            }

            sym.fpOffset = -autoSize;
         }
      }
      else {
         Error("Expected identifier in local variable declaration");
      }
      
      AddLocalSymbol(&sym);
      
      PrintSyntax("\n");
      GetToken(tok);
      
      ParseSemi(tok, "in local variable declaration");
   }
   
   // Function entry sequence
   EmitFunctionEntry(fn->name, autoSize, nRegister);

   // Function's executable code
   while (tok->token != TCBRACE) {
      ParseStatement(tok, fn, returnLabel, NOLABEL, NOLABEL);
   }
   
   GetToken(tok);
   
   // Function exit sequence
   EmitFunctionExit(returnLabel, nRegister);
   
   for (i = 0; i < NextStr; i++) {
      EmitStaticCharArray(&Strings[i], "<anon>");
   }
   
   NextStr = 0;
   ForgetLocalSymbols();
}


/* ParseStatement --- parse a single statement */

void ParseStatement(struct Token *tok, const struct Symbol *const fn, const int returnLabel, const int breakLabel, const int continueLabel)
{
   PrintSyntax("<statement> ");
   
   switch (tok->token) {
   case TRETURN:
      ParseReturn(tok, fn, returnLabel);
      break;
   case TIF:
      ParseIf(tok, fn, returnLabel, breakLabel, continueLabel);
      break;
   case TDO:
      ParseDo(tok, fn, returnLabel);
      break;
   case TFOR:
      ParseFor(tok, fn, returnLabel);
      break;
   case TWHILE:
      ParseWhile(tok, fn, returnLabel);
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
      ParseSwitch(tok, fn, returnLabel, continueLabel);
      break;
   case TOBRACE:
      ParseCompoundStatement(tok, fn, returnLabel, breakLabel, continueLabel);
      break;
   default:
      ParseExpression(tok);
      ParseSemi(tok, "after expression");
      break;
   }
}


/* ParseReturn --- parse a 'return' statement */

void ParseReturn(struct Token *tok, const struct Symbol *const fn, const int returnLabel)
{
   PrintSyntax("<return> ");
   GetToken(tok);

   if (tok->token == TSEMI) {
      PrintSyntax("\n");
      
      if (fn->type != T_VOID) {
         Error("non-void function %s returns no value", fn->name);
      }
   }
   else {
      ParseExpression(tok);
      
      if (fn->type == T_VOID) {
         Error("void function %s returns a value", fn->name);
      }
   }
   
   EmitJump(returnLabel, "return");

   ParseSemi(tok, "at end of 'return'");
}


/* ParseCompoundStatement --- parse a compound statement */

void ParseCompoundStatement(struct Token *tok, const struct Symbol *const fn, const int returnLabel, const int breakLabel, const int continueLabel)
{
   PrintSyntax("<compound_statement>\n");

   GetToken(tok);
   
   while (tok->token != TCBRACE) {
      ParseStatement(tok, fn, returnLabel, breakLabel, continueLabel);
   }

   GetToken(tok); /* Skip the closing curly bracket */
}


/* ParseIf --- parse an 'if' statement */

void ParseIf(struct Token *tok, const struct Symbol *const fn, const int returnLabel, const int breakLabel, const int continueLabel)
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
         ParseStatement(tok, fn, returnLabel, breakLabel, continueLabel);
         
         if (tok->token == TELSE) {
            const int endifLabel = AllocLabel('I');

            GetToken(tok);
            EmitJump(endifLabel, "if: jump to endif");
            EmitLabel(elseLabel);
            ParseStatement(tok, fn, returnLabel, breakLabel, continueLabel);
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
      struct Symbol tmp;
      struct Symbol *stp = NULL;
      
      tmp.storageClass = SCEXTERN;
      strncpy(tmp.name, tok->str, sizeof (tmp.name));
      tmp.type = T_INT;
      tmp.pLevel = 0;
      tmp.label = 0;
      tmp.fpOffset = 0;
      tmp.readOnly = false;
      
      if ((stp = LookUpLocalSymbol(tok->str)) == NULL) {
         stp = LookUpExternSymbol(tok->str);
         
         if (stp == NULL) {
            Error("Undeclared identifier: %s", tok->str);
         }
      }

      GetToken(tok);

      if ((tok->token == TINC) || (tok->token == TDEC)) {
         if (stp->readOnly) {
            Error("inc/dec 'const' object %s", stp->name);
         }
         else {
            LoadScalar(stp);

            if (tok->token == TINC) {
               EmitIncScalar(stp, 1);
            }
            else if (tok->token == TDEC) {
               EmitIncScalar(stp, -1);
            }
         }

         GetToken(tok);
      }
      else if (tok->token == TOPAREN) {
         int stackedBytes = 0;
         
         GetToken(tok);

         if (tok->token == TCPAREN) {
            EmitCallFunction(tmp.name, "call function no actual parameters");
            GetToken(tok);
         }
         else {
            do {
               ParseExpression(tok);
               
               if (tok->token == TCOMMA) {
                  GetToken(tok);
               }
               
               // TODO: actual parameters other than 2 bytes long
               Emit("pshs", "d", "<actual parameter>");
               stackedBytes += 2;
            } while (tok->token != TCPAREN);
         
            EmitCallFunction(tmp.name, "call function with parameters");
            GetToken(tok);
            //Error("Expected ')' in function call");
         }
         
         EmitStackCleanup(stackedBytes);
      }
      else if (tok->token == TASSIGN) {
         GetToken(tok);
         ParseExpression(tok);
         if (stp->readOnly) {
            Error("Assignment to 'const' object %s", stp->name);
         }
         else {
            StoreScalar(stp);
         }
      }
      else {
         LoadScalar(stp);
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
   else if (tok->token == TCOMMA) {
   }
   else if (tok->token == TCPAREN) {
   }
   else {
      Emit("nop", "", "<expression>");
      GetToken(tok);
   }
   
   PrintSyntax("\n");
}


/* ParseDo --- parse a 'do' statement */

void ParseDo(struct Token *tok, const struct Symbol *const fn, const int returnLabel)
{
   const int blabel = AllocLabel('b');
   const int clabel = AllocLabel('c');
   const int dlabel = AllocLabel('d');
   
   PrintSyntax("<do>\n");
   EmitLabel(dlabel);

   GetToken(tok);
   ParseStatement(tok, fn, returnLabel, blabel, clabel);

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

void ParseWhile(struct Token *tok, const struct Symbol *const fn, const int returnLabel)
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
         
         ParseStatement(tok, fn, returnLabel, blabel, clabel);
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

void ParseFor(struct Token *tok, const struct Symbol *const fn, const int returnLabel)
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
         ParseStatement(tok, fn, returnLabel, blabel, clabel);
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

void ParseSwitch(struct Token *tok, const struct Symbol *const fn, const int returnLabel, const int continueLabel)
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
                  ParseStatement(tok, fn, returnLabel, blabel, continueLabel);
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
   
   PrintSyntax("<const_int_expr>");
   
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
      
      PrintSyntax("<binop>");
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

