/* parser --- C parser                                      2022-07-31 */
/* Copyright (c) 2022 John Honniball. All rights reserved              */

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#define EOS   ('\0')

#define MAXNAME  (32)
#define MAXSYMS  (512)


enum eToken {
   TNULL, TSEMI, TCOLON, TCOMMA, TQUEST,
   TOPAREN, TCPAREN, TOSQBRK, TCSQBRK, TOBRACE, TCBRACE,
   TPLUS, TMINUS, TSTAR, TDIV, TMOD,
   TPLUSAB, TMINUSAB, TTIMESAB, TDIVAB, TMODAB,
   TASSIGN, TINC, TDEC,
   TOR, TAND, TEXOR, TNOT,
   TORAB, TANDAB, TEXORAB,
   TLOGAND, TLOGOR, TLOGNOT,
   TLE, TGE, TGT, TLT, TEQ, TNE,
   TLSHT, TRSHT, TLSHTAB, TRSHTAB,
   TPOINT, TDOT,
   TID, TINTLIT, TFLOATLIT, TSTRLIT,
   TBREAK, TCONTINUE, TDO, TFOR, TWHILE, TRETURN, TIF, TELSE, TGOTO,
   TCASE, TSWITCH, TDEFAULT, TSIZEOF,
   TAUTO, TREGISTER, TSTATIC, TEXTERN,
   TCONST, TVOLATILE, TTYPEDEF,
   TINT, TFLOAT, TDOUBLE, TCHAR, TSHORT, TLONG, TUNSIGNED, TSIGNED, TVOID,
   TENUM, TSTRUCT, TUNION,
};

enum eTokenType {
   KNULL,
   KTOKEN,
   KINVALID,
   KNAME,
   KNUMBER,
   KSTRING,
   KKEYWORD,
   KOP,
};

struct Token {
   int/**/type;
   int/**/iValue;
   double/*/*/fValue;
   enum/***/eToken/****/token;
   char/* */str[256];
};

struct Symbol {
   char name[MAXNAME];
   enum eToken token;
   bool isType;
   bool isKeyword;
};

int NextSym = 0;
struct Symbol SymTab[MAXSYMS];

void initialise(void);
void parse(const char fname[]);
void parser(const char fname[], FILE *fp);
int GetToken(FILE *fp, struct Token *tok);
void PrintToken(const struct Token *tok);
void installkw(const char keyword[], enum eToken token, bool isType);
enum eToken lookupKeyword(const char name[]);


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


void initialise(void)
{
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


void parse(const char fname[])
{
   FILE *fp;

   if ((fp = fopen(fname, "r")) == NULL) {
      fprintf(stderr, "%s: can't open\n", fname);
      return;
   }
   
   parser(fname, fp);

   fclose (fp);
}

void parser(const char fname[], FILE *fp)
{
   struct Token tok;

   while (GetToken(fp, &tok) != EOF)
      PrintToken(&tok);
}


int GetToken(FILE *fp, struct Token *tok)
{
   int ch;
   int i;
   static int state = 0;

   tok->token = TNULL;
   tok->iValue = 0;
   tok->fValue = 0.0;
   tok->type = KINVALID;
   tok->str[0] = EOS;
   
   while (1) {
      if ((ch = getc(fp)) == EOF)
         return (EOF);

      switch (state) {
      case 0:
         switch (ch) {
         case ':':
            tok->token = TCOLON;
            tok->type = KTOKEN;
            tok->str[0] = ch;
            tok->str[1] = EOS;
            return (tok->type);
         case ';':
            tok->token = TSEMI;
            tok->type = KTOKEN;
            tok->str[0] = ch;
            tok->str[1] = EOS;
            return (tok->type);
         case ',':
            tok->token = TCOMMA;
            tok->type = KTOKEN;
            tok->str[0] = ch;
            tok->str[1] = EOS;
            return (tok->type);
         case '(':
            tok->token = TOPAREN;
            tok->type = KTOKEN;
            tok->str[0] = ch;
            tok->str[1] = EOS;
            return (tok->type);
         case ')':
            tok->token = TCPAREN;
            tok->type = KTOKEN;
            tok->str[0] = ch;
            tok->str[1] = EOS;
            return (tok->type);
         case '[':
            tok->token = TOSQBRK;
            tok->type = KTOKEN;
            tok->str[0] = ch;
            tok->str[1] = EOS;
            return (tok->type);
         case ']':
            tok->token = TCSQBRK;
            tok->type = KTOKEN;
            tok->str[0] = ch;
            tok->str[1] = EOS;
            return (tok->type);
         case '{':
            tok->token = TOBRACE;
            tok->type = KTOKEN;
            tok->str[0] = ch;
            tok->str[1] = EOS;
            return (tok->type);
         case '}':
            tok->token = TCBRACE;
            tok->type = KTOKEN;
            tok->str[0] = ch;
            tok->str[1] = EOS;
            return (tok->type);
         case '.':
            tok->token = TDOT;
            tok->type = KOP;
            tok->str[0] = ch;
            tok->str[1] = EOS;
            return (tok->type);
         case '?':
            tok->token = TQUEST;
            tok->type = KOP;
            tok->str[0] = ch;
            tok->str[1] = EOS;
            return (tok->type);
         case '~':
            tok->token = TNOT;
            tok->type = KOP;
            tok->str[0] = ch;
            tok->str[1] = EOS;
            return (tok->type);
         case '#':
         case '$':
         case '`':
         case '@':
         case '\\':
            tok->type = KINVALID;
            tok->str[0] = ch;
            tok->str[1] = EOS;
            return (tok->type);
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
         case '0': case '1': case '2': case '3': case '4':
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
            ungetc(ch, fp);
            tok->str[i] = EOS;
            if ((token = lookupKeyword(tok->str)) == TNULL) {
               tok->token = TID;
               tok->type = KNAME;
            }
            else {
               tok->token = token;
               tok->type = KKEYWORD;
            }
            return (tok->type);
         }
         break;
      case 2:        // seen '|'
         if (ch == '|') {
            state = 0;
            tok->str[i++] = ch;
            tok->str[i] = EOS;
            tok->token = TLOGOR;
            tok->type = KOP;
            return (tok->type);
         }
         else if (ch == '=') {
            state = 0;
            tok->str[i++] = ch;
            tok->str[i] = EOS;
            tok->token = TORAB;
            tok->type = KOP;
            return (tok->type);
         }
         else {
            state = 0;
            ungetc(ch, fp);
            tok->str[i] = EOS;
            tok->token = TOR;
            tok->type = KOP;
            return (tok->type);
         }
         break;
      case 3:        // seen '='
         if (ch == '=') {
            state = 0;
            tok->str[i++] = ch;
            tok->str[i] = EOS;
            tok->token = TEQ;
            tok->type = KOP;
            return (tok->type);
         }
         else {
            state = 0;
            ungetc(ch, fp);
            tok->str[i] = EOS;
            tok->token = TASSIGN;
            tok->type = KOP;
            return (tok->type);
         }
         break;
      case 4:        // Seen '0' to '9'
         if ((ch >= '0') && (ch <= '9')) {
            tok->str[i++] = ch;
         }
         else {
            state = 0;
            ungetc(ch, fp);
            tok->str[i] = EOS;
            tok->token = TINTLIT;
            tok->iValue = atoi(tok->str);
            tok->type = KNUMBER;
            return (tok->type);
         }
         break;
      case 5:        // seen '*'
         if (ch == '=') {
            state = 0;
            tok->str[i++] = ch;
            tok->str[i] = EOS;
            tok->token = TTIMESAB;
            tok->type = KOP;
            return (tok->type);
         }
         else {
            state = 0;
            ungetc(ch, fp);
            tok->str[i] = EOS;
            tok->token = TSTAR;
            tok->type = KOP;
            return (tok->type);
         }
         break;
      case 6:        // seen '!'
         if (ch == '=') {
            state = 0;
            tok->str[i++] = ch;
            tok->str[i] = EOS;
            tok->token = TNE;
            tok->type = KOP;
            return (tok->type);
         }
         else {
            state = 0;
            ungetc(ch, fp);
            tok->str[i] = EOS;
            tok->token = TLOGNOT;
            tok->type = KOP;
            return (tok->type);
         }
         break;
      case 7:        // seen '+'
         if (ch == '=') {
            state = 0;
            tok->token = TPLUSAB;
            tok->str[i++] = ch;
            tok->str[i] = EOS;
            tok->type = KOP;
            return (tok->type);
         }
         else if (ch == '+') {
            state = 0;
            tok->token = TINC;
            tok->str[i++] = ch;
            tok->str[i] = EOS;
            tok->type = KOP;
            return (tok->type);
         }
         else {
            state = 0;
            ungetc(ch, fp);
            tok->token = TPLUS;
            tok->str[i] = EOS;
            tok->type = KOP;
            return (tok->type);
         }
         break;
      case 8:        // seen '-'
         if (ch == '=') {
            state = 0;
            tok->token = TMINUSAB;
            tok->str[i++] = ch;
            tok->str[i] = EOS;
            tok->type = KOP;
            return (tok->type);
         }
         else if (ch == '-') {
            state = 0;
            tok->token = TDEC;
            tok->str[i++] = ch;
            tok->str[i] = EOS;
            tok->type = KOP;
            return (tok->type);
         }
         else if (ch == '>') {
            state = 0;
            tok->token = TPOINT;
            tok->str[i++] = ch;
            tok->str[i] = EOS;
            tok->type = KOP;
            return (tok->type);
         }
         else {
            state = 0;
            tok->token = TMINUS;
            ungetc(ch, fp);
            tok->str[i] = EOS;
            tok->type = KOP;
            return (tok->type);
         }
         break;
      case 9:        // seen '&'
         if (ch == '=') {
            state = 0;
            tok->token = TANDAB;
            tok->str[i++] = ch;
            tok->str[i] = EOS;
            tok->type = KOP;
            return (tok->type);
         }
         else if (ch == '&') {
            state = 0;
            tok->token = TLOGAND;
            tok->str[i++] = ch;
            tok->str[i] = EOS;
            tok->type = KOP;
            return (tok->type);
         }
         else {
            state = 0;
            ungetc(ch, fp);
            tok->token = TAND;
            tok->str[i] = EOS;
            tok->type = KOP;
            return (tok->type);
         }
      case 10:       // seen '/'
         if (ch == '=') {
            state = 0;
            tok->token = TDIVAB;
            tok->str[i++] = ch;
            tok->str[i] = EOS;
            tok->type = KOP;
            return (tok->type);
         }
         else if (ch == '/') {
            state = 11;
         }
         else if (ch == '*') {
            state = 19;
         }
         else {
            state = 0;
            ungetc(ch, fp);
            tok->str[i] = EOS;
            tok->token = TDIV;
            tok->type = KOP;
            return (tok->type);
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
            tok->type = KSTRING;
            return (tok->type);
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
            tok->type = KOP;
            return (tok->type);
         }
         else {
            state = 0;
            ungetc(ch, fp);
            tok->token = TMOD;
            tok->str[i] = EOS;
            tok->type = KOP;
            return (tok->type);
         }
         break;
      case 15:       // seen '>'
         if (ch == '=') {
            state = 0;
            tok->str[i++] = ch;
            tok->str[i] = EOS;
            tok->token = TGE;
            tok->type = KOP;
            return (tok->type);
         }
         else if (ch == '>') {
            state = 16;
            tok->str[i++] = ch;
            tok->str[i] = EOS;
         }
         else {
            state = 0;
            ungetc(ch, fp);
            tok->str[i] = EOS;
            tok->token = TGT;
            tok->type = KOP;
            return (tok->type);
         }
         break;
      case 16:       // seen '>>'
         if (ch == '=') {
            state = 0;
            tok->str[i++] = ch;
            tok->str[i] = EOS;
            tok->token = TRSHTAB;
            tok->type = KOP;
            return (tok->type);
         }
         else {
            state = 0;
            ungetc(ch, fp);
            tok->str[i] = EOS;
            tok->token = TRSHT;
            tok->type = KOP;
            return (tok->type);
         }
         break;
      case 17:       // seen '<'
         if (ch == '=') {
            state = 0;
            tok->str[i++] = ch;
            tok->str[i] = EOS;
            tok->token = TLE;
            tok->type = KOP;
            return (tok->type);
         }
         else if (ch == '<') {
            state = 18;
            tok->str[i++] = ch;
            tok->str[i] = EOS;
         }
         else {
            state = 0;
            ungetc(ch, fp);
            tok->str[i] = EOS;
            tok->token = TLT;
            tok->type = KOP;
            return (tok->type);
         }
         break;
      case 18:       // seen '<<'
         if (ch == '=') {
            state = 0;
            tok->str[i++] = ch;
            tok->str[i] = EOS;
            tok->token = TLSHTAB;
            tok->type = KOP;
            return (tok->type);
         }
         else {
            state = 0;
            ungetc(ch, fp);
            tok->str[i] = EOS;
            tok->token = TLSHT;
            tok->type = KOP;
            return (tok->type);
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
            tok->type = KNUMBER;
            return (tok->type);
         }
         else {
            state = 23;
         }
         break;
      }
   }

   tok->type = KNULL;
   tok->str[0] = ch;
   tok->str[1] = EOS;

   return (tok->type);
}


/* PrintToken --- print a token for debugging or tracing */

void PrintToken(const struct Token *tok)
{
   switch (tok->type)
   {
   case KNULL:
      putc(tok->str[0], stdout);
      break;
   case KTOKEN:
      printf("TOKEN: '%s' %d\n", tok->str, tok->token);
      break;
   case KINVALID:
      printf("INVALID: '%c'\n", tok->str[0]);
      break;
   case KNAME:
      printf("NAME: '%s' %d\n", tok->str, tok->token);
      break;
   case KNUMBER:
      printf("NUMBER: '%s' %d %d\n", tok->str, tok->token, tok->iValue);
      break;
   case KSTRING:
      printf("STRING: '%s' %d\n", tok->str, tok->token);
      break;
   case KKEYWORD:
      printf("KEYWORD: '%s' %d\n", tok->str, tok->token);
      break;
   case KOP:
      printf("OPERATOR: '%s' %d\n", tok->str, tok->token);
      break;
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


/* lookupKeyword --- look up a name in the symbol table to see if it's a keyword */

enum eToken lookupKeyword(const char name[])
{
   int i;
   
   for (i = 0; (i < NextSym) && SymTab[i].isKeyword; i++)
     if (strcmp(name, SymTab[i].name) == 0)
        return (SymTab[i].token);
        
   return (TNULL);
}

