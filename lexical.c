/* lexical --- lexical analyser module                      2022-08-15 */
/* Copyright (c) 2022 John Honniball. All rights reserved              */

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "lexical.h"


#define EOS   ('\0')

#define MAXNAME  (32)
#define MAXSYMS  (512)

struct Symbol {
   char name[MAXNAME];
   enum eToken token;
   bool isType;
   bool isKeyword;
};

static int NextSym = 0;
static struct Symbol SymTab[MAXSYMS];
static FILE *Src = NULL;
static bool TraceTokens = false;
static bool TraceSyntax = false;

static int GetOneToken(struct Token *tok);


/* LexicalInit --- initialise this module */

void LexicalInit(void)
{
   TraceTokens = false;
   
   installkw("break",    TBREAK,    false);
   installkw("case",     TCASE,     false);
   installkw("continue", TCONTINUE, false);
   installkw("do",       TDO,       false);
   installkw("default",  TDEFAULT,  false);
   installkw("else",     TELSE,     false);
   installkw("for",      TFOR,      false);
   installkw("goto",     TGOTO,     false);
   installkw("if",       TIF,       false);
   installkw("typedef",  TTYPEDEF,  false);
   installkw("return",   TRETURN,   false);
   installkw("sizeof",   TSIZEOF,   false);
   installkw("switch",   TSWITCH,   false);
   installkw("while",    TWHILE,    false);
   
   installkw("int",      TINT,      true);
   installkw("float",    TFLOAT,    true);
   installkw("double",   TDOUBLE,   true);
   installkw("char",     TCHAR,     true);
   installkw("void",     TVOID,     true);
   installkw("short",    TSHORT,    true);
   installkw("long",     TLONG,     true);
   installkw("auto",     TAUTO,     true);
   installkw("static",   TSTATIC,   true);
   installkw("const",    TCONST,    true);
   installkw("volatile", TVOLATILE, true);
   installkw("signed",   TSIGNED,   true);
   installkw("unsigned", TUNSIGNED, true);
   installkw("register", TREGISTER, true);
   installkw("extern",   TEXTERN,   true);
   installkw("struct",   TSTRUCT,   true);
   installkw("union",    TUNION,    true);
   installkw("enum",     TENUM,     true);
}


/* SetTokenTraceFlag --- set or clear the lexical token trace flag */

void SetTokenTraceFlag(const bool enabled)
{
   TraceTokens = enabled;
}


/* SetSyntaxTraceFlag --- set or clear the syntax/parser trace flag */

void SetSyntaxTraceFlag(const bool enabled)
{
   TraceSyntax = enabled;
}


/* PrintSyntax --- print a syntax trace message, if enabled */

void PrintSyntax(const char msg[])
{
   if (TraceSyntax) {
      fputs(msg, stdout);
   }
}


/* installkw --- install a C keyword in the symbol table */

void installkw(const char keyword[], enum eToken token, bool isType)
{
   struct Symbol *sym = &SymTab[NextSym++];
   
   strcpy(sym->name, keyword);
   sym->token     = token;
   sym->isType    = isType;
   sym->isKeyword = true;
}


/* OpenSourceFile --- open the C source code file */

bool OpenSourceFile(const char fname[])
{
   FILE *fp;

   if ((fp = fopen(fname, "r")) == NULL) {
      fprintf(stderr, "%s: can't open\n", fname);
      return (false);
   }
   
   Src = fp;
   
   return (true);
}


/* CloseSourceFile --- close the source code file */

bool CloseSourceFile(void)
{
   if (Src != NULL)
      fclose(Src);
   
   return (true);
}


/* lookupKeyword --- look up a name in the symbol table to see if it's a keyword */

enum eToken lookupKeyword(const char name[])
{
   int i;
   
   for (i = 0; (i < NextSym) && SymTab[i].isKeyword; i++)
     if (strcmp(name, SymTab[i].name) == 0)
        return (SymTab[i].token);
        
   return (TNULL);
}



/* PrintToken --- print a token for debugging or tracing */

void PrintToken(const struct Token *tok)
{
   switch (tok->token) {
   case TNULL:
      printf("NULL:     '%c'\n", tok->str[0]);
      break;
   case TINVAL:
      printf("INVALID:  '%c'\n", tok->str[0]);
      break;
   case TID:
      printf("NAME:     '%s' %s\n", tok->str, tokenAsStr(tok));
      break;
   case TINTLIT:
      printf("NUMBER:   '%s' %s %d\n", tok->str, tokenAsStr(tok), tok->iValue);
      break;
   case TFLOATLIT:
      printf("NUMBER:   '%s' %s %g\n", tok->str, tokenAsStr(tok), tok->fValue);
      break;
   case TSTRLIT:
      printf("STRING:   '%s' %s\n", tok->str, tokenAsStr(tok));
      break;
   case TBREAK:
   case TCONTINUE:
   case TDO:
   case TFOR:
   case TWHILE:
   case TRETURN:
   case TIF:
   case TELSE:
   case TGOTO:
   case TCASE:
   case TSWITCH:
   case TDEFAULT:
   case TSIZEOF:
   case TAUTO:
   case TREGISTER:
   case TSTATIC:
   case TEXTERN:
   case TCONST:
   case TVOLATILE:
   case TTYPEDEF:
   case TINT:
   case TFLOAT:
   case TDOUBLE:
   case TCHAR:
   case TSHORT:
   case TLONG:
   case TUNSIGNED:
   case TSIGNED:
   case TVOID:
   case TENUM:
   case TSTRUCT:
   case TUNION:
      printf("KEYWORD:  '%s' %s\n", tok->str, tokenAsStr(tok)); 
      break;
   case TPLUS:
   case TMINUS:
   case TSTAR:
   case TDIV:
   case TMOD:
   case TPLUSAB:
   case TMINUSAB:
   case TTIMESAB:
   case TDIVAB:
   case TMODAB:
   case TASSIGN:
   case TINC:
   case TDEC:
   case TOR:
   case TAND:
   case TEXOR:
   case TNOT:
   case TORAB:
   case TANDAB:
   case TEXORAB:
   case TLOGAND:
   case TLOGOR:
   case TLOGNOT:
   case TLE:
   case TGE:
   case TGT:
   case TLT:
   case TEQ:
   case TNE:
   case TLSHT:
   case TRSHT:
   case TLSHTAB:
   case TRSHTAB:
   case TPOINT:
   case TDOT:
   case TQUEST:
      printf("OPERATOR: '%s' %s\n", tok->str, tokenAsStr(tok));
      break;
   default:
      printf("TOKEN:    '%s' %s\n", tok->str, tokenAsStr(tok));
      break;
   }
}


/* GetToken --- get the a token and print trace if enabled */

int GetToken(struct Token *tok)
{
   GetOneToken(tok);

   if (TraceTokens) {
      PrintToken(tok);
   }
   
   return (tok->token);
}


/* GetOneToken --- get the next lexical token from the source file */

static int GetOneToken(struct Token *tok)
{
   int ch;
   int i;
   static int state = 0;

   tok->token = TNULL;
   tok->iValue = 0;
   tok->fValue = 0.0;
   tok->str[0] = EOS;
   
   while (1) {
      if ((ch = getc(Src)) == EOF) {
         tok->token = TEOF;
         return (tok->token);
      }

      switch (state) {
      case 0:
         switch (ch) {
         case ':':
            tok->token = TCOLON;
            tok->str[0] = ch;
            tok->str[1] = EOS;
            return (tok->token);
         case ';':
            tok->token = TSEMI;
            tok->str[0] = ch;
            tok->str[1] = EOS;
            return (tok->token);
         case ',':
            tok->token = TCOMMA;
            tok->str[0] = ch;
            tok->str[1] = EOS;
            return (tok->token);
         case '(':
            tok->token = TOPAREN;
            tok->str[0] = ch;
            tok->str[1] = EOS;
            return (tok->token);
         case ')':
            tok->token = TCPAREN;
            tok->str[0] = ch;
            tok->str[1] = EOS;
            return (tok->token);
         case '[':
            tok->token = TOSQBRK;
            tok->str[0] = ch;
            tok->str[1] = EOS;
            return (tok->token);
         case ']':
            tok->token = TCSQBRK;
            tok->str[0] = ch;
            tok->str[1] = EOS;
            return (tok->token);
         case '{':
            tok->token = TOBRACE;
            tok->str[0] = ch;
            tok->str[1] = EOS;
            return (tok->token);
         case '}':
            tok->token = TCBRACE;
            tok->str[0] = ch;
            tok->str[1] = EOS;
            return (tok->token);
         case '.':
            tok->token = TDOT;
            tok->str[0] = ch;
            tok->str[1] = EOS;
            return (tok->token);
         case '?':
            tok->token = TQUEST;
            tok->str[0] = ch;
            tok->str[1] = EOS;
            return (tok->token);
         case '~':
            tok->token = TNOT;
            tok->str[0] = ch;
            tok->str[1] = EOS;
            return (tok->token);
         case '#':
         case '$':
         case '`':
         case '@':
         case '\\':
            tok->token = TINVAL;
            tok->str[0] = ch;
            tok->str[1] = EOS;
            return (tok->token);
         case 'a': case 'b': case 'c': case 'd': case 'e': case 'f': case 'g': case 'h': case 'i': case 'j':
         case 'k': case 'l': case 'm': case 'n': case 'o': case 'p': case 'q': case 'r': case 's': case 't':
         case 'u': case 'v': case 'w': case 'x': case 'y': case 'z':
         case 'A': case 'B': case 'C': case 'D': case 'E': case 'F': case 'G': case 'H': case 'I': case 'J':
         case 'K': case 'L': case 'M': case 'N': case 'O': case 'P': case 'Q': case 'R': case 'S': case 'T':
         case 'U': case 'V': case 'W': case 'X': case 'Y': case 'Z':
         case '_':
            tok->str[0] = ch;
            i = 1;
            state = 1;
            break;
         case '|':
            tok->str[0] = ch;
            i = 1;
            state = 2;
            break;
         case '=':
            tok->str[0] = ch;
            i = 1;
            state = 3;
            break;
         case '0':
            tok->str[0] = ch;
            i = 1;
            state = 24;
            break;
         case '1': case '2': case '3': case '4':
         case '5': case '6': case '7': case '8': case '9':
            tok->str[0] = ch;
            i = 1;
            state = 4;
            break;
         case '*':
            tok->str[0] = ch;
            i = 1;
            state = 5;
            break;
         case '!':
            tok->str[0] = ch;
            i = 1;
            state = 6;
            break;
         case '+':
            tok->str[0] = ch;
            i = 1;
            state = 7;
            break;
         case '-':
            tok->str[0] = ch;
            i = 1;
            state = 8;
            break;
         case '&':
            tok->str[0] = ch;
            i = 1;
            state = 9;
            break;
         case '/':
            tok->str[0] = ch;
            i = 1;
            state = 10;
            break;
         case '"':
            i = 0;
            state = 12;
            break;
         case '%':
            tok->str[0] = ch;
            i = 1;
            state = 14;
            break;
         case '>':
            tok->str[0] = ch;
            i = 1;
            state = 15;
            break;
         case '<':
            tok->str[0] = ch;
            i = 1;
            state = 17;
            break;
         case '\'':
            i = 0;
            state = 21;
            break;
         }
         break;
      case 1:        // name
         if (((ch >= 'a') && (ch <= 'z')) ||
             ((ch >= 'A') && (ch <= 'Z')) ||
             ((ch >= '0') && (ch <= '9')) ||
             (ch == '_')) {
            tok->str[i++] = ch;
         }
         else {
            enum eToken token;
            
            state = 0;
            ungetc(ch, Src);
            tok->str[i] = EOS;
            if ((token = lookupKeyword(tok->str)) == TNULL) {
               tok->token = TID;
            }
            else {
               tok->token = token;
            }
            return (tok->token);
         }
         break;
      case 2:        // seen '|'
         if (ch == '|') {
            state = 0;
            tok->str[i++] = ch;
            tok->str[i] = EOS;
            tok->token = TLOGOR;
            return (tok->token);
         }
         else if (ch == '=') {
            state = 0;
            tok->str[i++] = ch;
            tok->str[i] = EOS;
            tok->token = TORAB;
            return (tok->token);
         }
         else {
            state = 0;
            ungetc(ch, Src);
            tok->str[i] = EOS;
            tok->token = TOR;
            return (tok->token);
         }
         break;
      case 3:        // seen '='
         if (ch == '=') {
            state = 0;
            tok->str[i++] = ch;
            tok->str[i] = EOS;
            tok->token = TEQ;
            return (tok->token);
         }
         else {
            state = 0;
            ungetc(ch, Src);
            tok->str[i] = EOS;
            tok->token = TASSIGN;
            return (tok->token);
         }
         break;
      case 4:        // Seen '1' to '9' (decimal number)
         if ((ch >= '0') && (ch <= '9')) {
            tok->str[i++] = ch;
         }
         else if (ch == '.') {
            tok->str[i++] = ch;
            state = 27;
         }
         else if ((ch == 'e') || (ch == 'E')) {
            tok->str[i++] = ch;
            state = 28;
         }
         else if ((ch == 'L') || (ch == 'l')) {
            tok->str[i++] = ch;
         }
         else if ((ch == 'U') || (ch == 'u')) {
            tok->str[i++] = ch;
         }
         else {
            state = 0;
            ungetc(ch, Src);
            tok->str[i] = EOS;
            tok->token = TINTLIT;
            tok->iValue = atoi(tok->str);
            return (tok->token);
         }
         break;
      case 5:        // seen '*'
         if (ch == '=') {
            state = 0;
            tok->str[i++] = ch;
            tok->str[i] = EOS;
            tok->token = TTIMESAB;
            return (tok->token);
         }
         else {
            state = 0;
            ungetc(ch, Src);
            tok->str[i] = EOS;
            tok->token = TSTAR;
            return (tok->token);
         }
         break;
      case 6:        // seen '!'
         if (ch == '=') {
            state = 0;
            tok->str[i++] = ch;
            tok->str[i] = EOS;
            tok->token = TNE;
            return (tok->token);
         }
         else {
            state = 0;
            ungetc(ch, Src);
            tok->str[i] = EOS;
            tok->token = TLOGNOT;
            return (tok->token);
         }
         break;
      case 7:        // seen '+'
         if (ch == '=') {
            state = 0;
            tok->token = TPLUSAB;
            tok->str[i++] = ch;
            tok->str[i] = EOS;
            return (tok->token);
         }
         else if (ch == '+') {
            state = 0;
            tok->token = TINC;
            tok->str[i++] = ch;
            tok->str[i] = EOS;
            return (tok->token);
         }
         else {
            state = 0;
            ungetc(ch, Src);
            tok->token = TPLUS;
            tok->str[i] = EOS;
            return (tok->token);
         }
         break;
      case 8:        // seen '-'
         if (ch == '=') {
            state = 0;
            tok->token = TMINUSAB;
            tok->str[i++] = ch;
            tok->str[i] = EOS;
            return (tok->token);
         }
         else if (ch == '-') {
            state = 0;
            tok->token = TDEC;
            tok->str[i++] = ch;
            tok->str[i] = EOS;
            return (tok->token);
         }
         else if (ch == '>') {
            state = 0;
            tok->token = TPOINT;
            tok->str[i++] = ch;
            tok->str[i] = EOS;
            return (tok->token);
         }
         else {
            state = 0;
            tok->token = TMINUS;
            ungetc(ch, Src);
            tok->str[i] = EOS;
            return (tok->token);
         }
         break;
      case 9:        // seen '&'
         if (ch == '=') {
            state = 0;
            tok->token = TANDAB;
            tok->str[i++] = ch;
            tok->str[i] = EOS;
            return (tok->token);
         }
         else if (ch == '&') {
            state = 0;
            tok->token = TLOGAND;
            tok->str[i++] = ch;
            tok->str[i] = EOS;
            return (tok->token);
         }
         else {
            state = 0;
            ungetc(ch, Src);
            tok->token = TAND;
            tok->str[i] = EOS;
            return (tok->token);
         }
      case 10:       // seen '/'
         if (ch == '=') {
            state = 0;
            tok->token = TDIVAB;
            tok->str[i++] = ch;
            tok->str[i] = EOS;
            return (tok->token);
         }
         else if (ch == '/') {
            state = 11;
         }
         else if (ch == '*') {
            state = 19;
         }
         else {
            state = 0;
            ungetc(ch, Src);
            tok->str[i] = EOS;
            tok->token = TDIV;
            return (tok->token);
         }
         break;
      case 11:       // seen '//', comment to end-of-line
         if (ch == '\n') {
            state = 0;
         }
         break;
      case 12:       // seen '"', string literal
         if (ch == '\\') {
            state = 13;
         }
         else if (ch == '"') {
            state = 0;
            tok->str[i] = EOS;
            tok->token = TSTRLIT;
            return (tok->token);
         }
         else {
            tok->str[i++] = ch;
         }
         break;
      case 13:       // seen '\' within string literal
         switch (ch) {
         case 'a':
            tok->str[i++] = '\a';
            break;
         case 'n':
            tok->str[i++] = '\n';
            break;
         case 'r':
            tok->str[i++] = '\r';
            break;
         case 't':
            tok->str[i++] = '\t';
            break;
         case '\\':
            tok->str[i++] = '\\';
            break;
         case '"':
            tok->str[i++] = '"';
            break;
         default:
            tok->str[i++] = ch;
            break;
         }
         state = 12;
         break;
      case 14:       // seen '%'
         if (ch == '=') {
            state = 0;
            tok->token = TMODAB;
            tok->str[i++] = ch;
            tok->str[i] = EOS;
            return (tok->token);
         }
         else {
            state = 0;
            ungetc(ch, Src);
            tok->token = TMOD;
            tok->str[i] = EOS;
            return (tok->token);
         }
         break;
      case 15:       // seen '>'
         if (ch == '=') {
            state = 0;
            tok->str[i++] = ch;
            tok->str[i] = EOS;
            tok->token = TGE;
            return (tok->token);
         }
         else if (ch == '>') {
            state = 16;
            tok->str[i++] = ch;
            tok->str[i] = EOS;
         }
         else {
            state = 0;
            ungetc(ch, Src);
            tok->str[i] = EOS;
            tok->token = TGT;
            return (tok->token);
         }
         break;
      case 16:       // seen '>>'
         if (ch == '=') {
            state = 0;
            tok->str[i++] = ch;
            tok->str[i] = EOS;
            tok->token = TRSHTAB;
            return (tok->token);
         }
         else {
            state = 0;
            ungetc(ch, Src);
            tok->str[i] = EOS;
            tok->token = TRSHT;
            return (tok->token);
         }
         break;
      case 17:       // seen '<'
         if (ch == '=') {
            state = 0;
            tok->str[i++] = ch;
            tok->str[i] = EOS;
            tok->token = TLE;
            return (tok->token);
         }
         else if (ch == '<') {
            state = 18;
            tok->str[i++] = ch;
            tok->str[i] = EOS;
         }
         else {
            state = 0;
            ungetc(ch, Src);
            tok->str[i] = EOS;
            tok->token = TLT;
            return (tok->token);
         }
         break;
      case 18:       // seen '<<'
         if (ch == '=') {
            state = 0;
            tok->str[i++] = ch;
            tok->str[i] = EOS;
            tok->token = TLSHTAB;
            return (tok->token);
         }
         else {
            state = 0;
            ungetc(ch, Src);
            tok->str[i] = EOS;
            tok->token = TLSHT;
            return (tok->token);
         }
         break;
      case 19:
         if (ch == '*') {
            state = 20;
         }
         break;
      case 20:
         if (ch == '/') {  // End of traditional C comment
            state = 0;
         }
         else {
            state = 19;
         }
         break;
      case 21:
         if (ch == '\\') {
            state = 22;
         }
         else {
            tok->str[i++] = ch;
            tok->str[i] = EOS;
            state = 23;
         }
         break;
      case 22:
         if (ch == 'a') {
            tok->str[i++] = '\a';
            tok->str[i] = EOS;
            state = 23;
         }
         else if (ch == 'n') {
            tok->str[i++] = '\n';
            tok->str[i] = EOS;
            state = 23;
         }
         else if (ch == 'r') {
            tok->str[i++] = '\r';
            tok->str[i] = EOS;
            state = 23;
         }
         else if (ch == 't') {
            tok->str[i++] = '\t';
            tok->str[i] = EOS;
            state = 23;
         }
         else if (ch == '\'') {
            tok->str[i++] = '\'';
            tok->str[i] = EOS;
            state = 23;
         }
         else if (ch == '\\') {
            tok->str[i++] = '\\';
            tok->str[i] = EOS;
            state = 23;
         }
         else {
            tok->str[i++] = ch;
            tok->str[i] = EOS;
            state = 23;
         }
         break;
      case 23:
         if (ch == '\'') {
            state = 0;
            tok->iValue = tok->str[0];
            tok->token = TINTLIT;
            return (tok->token);
         }
         else {
            state = 23;
         }
         break;
      case 24:       // seen '0' (octal or hex number, or float number)
         if ((ch == 'x') || (ch == 'X')) {
            tok->str[i++] = ch;
            state = 26;
         }
         else if ((ch >= '0') && (ch <= '7')) {
            tok->str[i++] = ch;
            state = 25;
         }
         else if (ch == '.') {
            tok->str[i++] = ch;
            state = 27;
         }
         else if ((ch == 'L') || (ch == 'l')) {
            tok->str[i++] = ch;
         }
         else if ((ch == 'U') || (ch == 'u')) {
            tok->str[i++] = ch;
         }
         else {
            state = 0;
            ungetc(ch, Src);
            tok->str[i] = EOS;
            tok->token = TINTLIT;
            tok->iValue = strtoul(tok->str, NULL, 8);
            return (tok->token);
         }
         break;
      case 25:       // seen '0' followed by '0' to '7' (octal number)
         if ((ch >= '0') && (ch <= '7')) {
            tok->str[i++] = ch;
         }
         else if (ch == '.') {
            tok->str[i++] = ch;
            state = 27;
         }
         else if ((ch == 'L') || (ch == 'l')) {
            tok->str[i++] = ch;
         }
         else if ((ch == 'U') || (ch == 'u')) {
            tok->str[i++] = ch;
         }
         else {
            state = 0;
            ungetc(ch, Src);
            tok->str[i] = EOS;
            tok->token = TINTLIT;
            tok->iValue = strtoul(tok->str, NULL, 8);
            return (tok->token);
         }
         break;
      case 26:       // seen '0x' (hex number)
         if (((ch >= '0') && (ch <= '9')) ||
             ((ch >= 'a') && (ch <= 'f')) ||
             ((ch >= 'A') && (ch <= 'F'))) {
            tok->str[i++] = ch;
         }
         else if ((ch == 'L') || (ch == 'l')) {
            tok->str[i++] = ch;
         }
         else if ((ch == 'U') || (ch == 'u')) {
            tok->str[i++] = ch;
         }
         else {
            state = 0;
            ungetc(ch, Src);
            tok->str[i] = EOS;
            tok->token = TINTLIT;
            tok->iValue = strtoul(&tok->str[2], NULL, 16);
            return (tok->token);
         }
         break;
      case 27:       // seen '.' after decimal digits
         if ((ch >= '0') && (ch <= '9')) {
            tok->str[i++] = ch;
         }
         else if ((ch == 'e') || (ch == 'E')) {
            tok->str[i++] = ch;
            state = 28;
         }
         else if ((ch == 'f') || (ch == 'F')) {
            tok->str[i++] = ch;
            state = 0;
            tok->str[i] = EOS;
            tok->token = TFLOATLIT;
            tok->fValue = strtod(tok->str, NULL);
            return (tok->token);
         }
         else if ((ch == 'l') || (ch == 'L')) {
            tok->str[i++] = ch;
            state = 0;
            tok->str[i] = EOS;
            tok->token = TFLOATLIT;
            tok->fValue = strtod(tok->str, NULL);
            return (tok->token);
         }
         else {
            state = 0;
            ungetc(ch, Src);
            tok->str[i] = EOS;
            tok->token = TFLOATLIT;
            tok->fValue = strtod(tok->str, NULL);
            return (tok->token);
         }
         break;
      case 28:       // seen 'e' or 'E' after decimal digits
         if ((ch >= '0') && (ch <= '9')) {
            tok->str[i++] = ch;
         }
         else if ((ch == '+') || (ch == '-')) {
            tok->str[i++] = ch;
         }
         else if ((ch == 'f') || (ch == 'F')) {
            tok->str[i++] = ch;
            state = 0;
            tok->str[i] = EOS;
            tok->token = TFLOATLIT;
            tok->fValue = strtod(tok->str, NULL);
            return (tok->token);
         }
         else if ((ch == 'l') || (ch == 'L')) {
            tok->str[i++] = ch;
            state = 0;
            tok->str[i] = EOS;
            tok->token = TFLOATLIT;
            tok->fValue = strtod(tok->str, NULL);
            return (tok->token);
         }
         else {
            state = 0;
            ungetc(ch, Src);
            tok->str[i] = EOS;
            tok->token = TFLOATLIT;
            tok->fValue = strtod(tok->str, NULL);
            return (tok->token);
         }
         break;
      }
   }

   tok->str[0] = ch;
   tok->str[1] = EOS;

   return (tok->token);
}


/* tokenAsStr --- generate a string representation of any token */

char *tokenAsStr(const struct Token *tok)
{
   char *str;
   
   switch (tok->token) {
   case TNULL:
      str = "TNULL";
      break;
   case TSEMI:
      str = "TSEMI";
      break;
   case TCOLON:
      str = "TCOLON";
      break;
   case TCOMMA:
      str = "TCOMMA";
      break;
   case TQUEST:
      str = "TQUEST";
      break;
   case TOPAREN:
      str = "TOPAREN";
      break;
   case TCPAREN:
      str = "TCPAREN";
      break;
   case TOSQBRK:
      str = "TOSQBRK";
      break;
   case TCSQBRK:
      str = "TCSQBRK";
      break;
   case TOBRACE:
      str = "TOBRACE";
      break;
   case TCBRACE:
      str = "TCBRACE";
      break;
   case TPLUS:
      str = "TPLUS";
      break;
   case TMINUS:
      str = "TMINUS";
      break;
   case TSTAR:
      str = "TSTAR";
      break;
   case TDIV:
      str = "TDIV";
      break;
   case TMOD:
      str = "TMOD";
      break;
   case TPLUSAB:
      str = "TPLUSAB";
      break;
   case TMINUSAB:
      str = "TMINUSAB";
      break;
   case TTIMESAB:
      str = "TTIMESAB";
      break;
   case TDIVAB:
      str = "TDIVAB";
      break;
   case TMODAB:
      str = "TMODAB";
      break;
   case TASSIGN:
      str = "TASSIGN";
      break;
   case TINC:
      str = "TINC";
      break;
   case TDEC:
      str = "TDEC";
      break;
   case TOR:
      str = "TOR";
      break;
   case TAND:
      str = "TAND";
      break;
   case TEXOR:
      str = "TEXOR";
      break;
   case TNOT:
      str = "TNOT";
      break;
   case TORAB:
      str = "TORAB";
      break;
   case TANDAB:
      str = "TANDAB";
      break;
   case TEXORAB:
      str = "TEXORAB";
      break;
   case TLOGAND:
      str = "TLOGAND";
      break;
   case TLOGOR:
      str = "TLOGOR";
      break;
   case TLOGNOT:
      str = "TLOGNOT";
      break;
   case TLE:
      str = "TLE";
      break;
   case TGE:
      str = "TGE";
      break;
   case TGT:
      str = "TGT";
      break;
   case TLT:
      str = "TLT";
      break;
   case TEQ:
      str = "TEQ";
      break;
   case TNE:
      str = "TNE";
      break;
   case TLSHT:
      str = "TLSHT";
      break;
   case TRSHT:
      str = "TRSHT";
      break;
   case TLSHTAB:
      str = "TLSHTAB";
      break;
   case TRSHTAB:
      str = "TRSHTAB";
      break;
   case TPOINT:
      str = "TPOINT";
      break;
   case TDOT:
      str = "TDOT";
      break;
   case TID:
      str = "TID";
      break;
   case TINTLIT:
      str = "TINTLIT";
      break;
   case TFLOATLIT:
      str = "TFLOATLIT";
      break;
   case TSTRLIT:
      str = "TSTRLIT";
      break;
   case TBREAK:
      str = "TBREAK";
      break;
   case TCONTINUE:
      str = "TCONTINUE";
      break;
   case TDO:
      str = "TDO";
      break;
   case TFOR:
      str = "TFOR";
      break;
   case TWHILE:
      str = "TWHILE";
      break;
   case TRETURN:
      str = "TRETURN";
      break;
   case TIF:
      str = "TIF";
      break;
   case TELSE:
      str = "TELSE";
      break;
   case TGOTO:
      str = "TGOTO";
      break;
   case TCASE:
      str = "TCASE";
      break;
   case TSWITCH:
      str = "TSWITCH";
      break;
   case TDEFAULT:
      str = "TDEFAULT";
      break;
   case TSIZEOF:
      str = "TSIZEOF";
      break;
   case TAUTO:
      str = "TAUTO";
      break;
   case TREGISTER:
      str = "TREGISTER";
      break;
   case TSTATIC:
      str = "TSTATIC";
      break;
   case TEXTERN:
      str = "TEXTERN";
      break;
   case TCONST:
      str = "TCONST";
      break;
   case TVOLATILE:
      str = "TVOLATILE";
      break;
   case TTYPEDEF:
      str = "TTYPEDEF";
      break;
   case TINT:
      str = "TINT";
      break;
   case TFLOAT:
      str = "TFLOAT";
      break;
   case TDOUBLE:
      str = "TDOUBLE";
      break;
   case TCHAR:
      str = "TCHAR";
      break;
   case TSHORT:
      str = "TSHORT";
      break;
   case TLONG:
      str = "TLONG";
      break;
   case TUNSIGNED:
      str = "TUNSIGNED";
      break;
   case TSIGNED:
      str = "TSIGNED";
      break;
   case TVOID:
      str = "TVOID";
      break;
   case TENUM:
      str = "TENUM";
      break;
   case TSTRUCT:
      str = "TSTRUCT";
      break;
   case TUNION:
      str = "TUNION";
      break;
   case TEOF:
      str = "EOF";
      break;
   default:
      str = "ERROR";
      break;
   }
   
   return (str);
}
