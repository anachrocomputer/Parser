/* parser --- C parser                                      2022-07-31 */
/* Copyright (c) 2022 John Honniball. All rights reserved              */

#include <stdio.h>
#include <stdlib.h>

#define EOS   ('\0')

struct Token {
   int/**/type;
   int/**/value;
   char/* */str[256];
};

enum eToken {
   TNULL, TSEMI, TCOLON, TCOMMA, TQUEST,
   TOPAREN, TCPAREN, TOSQBRK, TCSQBRK, TOBRACE, TCBRACE,
   TPLUS, TMINUS, TSTAR, TDIV, TMOD,
   TPLUSAB, TMINUSAB, TTIMESAB, TDIVAB, TMODAB,
   TINC, TDEC,
   TOR, TAND, TEXOR, TNOT,
   TORAB, TANDAB, TEXORAB,
   TLOGAND, TLOGOR, TLOGNOT,
   TLE, TGE, TGT, TLT, TEQ, TNE,
   TLSHT, TRSHT, TLSHTAB, TRSHTAB,
   TPOINT, TDOT,
};

void parse(const char fname[]);
void parser(const char fname[], FILE *fp);
int GetToken(FILE *fp, struct Token *tok);
void PrintToken(const struct Token *tok);


int main(const int argc, const char *argv[])
{
   int i;

   for (i = 1; i < argc; i++) {
      if (argv[i][0] == '-') {
      }
      else {
         parse(argv[i]);
      }
   }

   return (0);
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

   while (1) {
      if ((ch = getc(fp)) == EOF)
         return (EOF);

      switch (state) {
      case 0:
         switch (ch) {
         case ':':
         case ';':
         case ',':
         case '(':
         case ')':
         case '[':
         case ']':
         case '{':
         case '}':
         case '.':
         case '?':
         case '~':
            tok->type = 1;
            tok->str[0] = ch;
            tok->str[1] = EOS;
            return (tok->type);
         case '#':
         case '$':
         case '`':
         case '@':
         case '\\':
            tok->type = 2;
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
            state = 0;
            ungetc(ch, fp);
            tok->str[i] = EOS;
            tok->type = 3;
            return (tok->type);
         }
         break;
      case 2:
         if ((ch == '|') || (ch == '=')) {
            state = 0;
            tok->str[i++] = ch;
            tok->str[i] = EOS;
            tok->type = 1;
            return (tok->type);
         }
         else {
            state = 0;
            ungetc(ch, fp);
            tok->str[i] = EOS;
            tok->type = 1;
            return (tok->type);
         }
         break;
      case 3:
         if (ch == '=') {
            state = 0;
            tok->str[i++] = ch;
            tok->str[i] = EOS;
            tok->type = 1;
            return (tok->type);
         }
         else if (ch == '>') {
            state = 0;
            tok->str[i++] = ch;
            tok->str[i] = EOS;
            tok->type = 1;
            return (tok->type);
         }
         else if (ch == '<') {
            state = 0;
            tok->str[i++] = ch;
            tok->str[i] = EOS;
            tok->type = 1;
            return (tok->type);
         }
         else {
            state = 0;
            ungetc(ch, fp);
            tok->str[i] = EOS;
            tok->type = 1;
            return (tok->type);
         }
         break;
      case 4:
         if ((ch >= '0') && (ch <= '9')) {
            tok->str[i++] = ch;
         }
         else {
            state = 0;
            ungetc(ch, fp);
            tok->str[i] = EOS;
            tok->value = atoi(tok->str);
            tok->type = 4;
            return (tok->type);
         }
         break;
      case 5:
         if (ch == '=') {
            state = 0;
            tok->str[i++] = ch;
            tok->str[i] = EOS;
            tok->type = 1;
            return (tok->type);
         }
         else {
            state = 0;
            ungetc(ch, fp);
            tok->str[i] = EOS;
            tok->type = 1;
            return (tok->type);
         }
         break;
      case 6:
         if (ch == '=') {
            state = 0;
            tok->str[i++] = ch;
            tok->str[i] = EOS;
            tok->type = 1;
            return (tok->type);
         }
         else {
            state = 0;
            ungetc(ch, fp);
            tok->str[i] = EOS;
            tok->type = 1;
            return (tok->type);
         }
         break;
      case 7:
         if (ch == '=') {
            state = 0;
            tok->str[i++] = ch;
            tok->str[i] = EOS;
            tok->type = 1;
            return (tok->type);
         }
         else if (ch == '+') {
            state = 0;
            tok->str[i++] = ch;
            tok->str[i] = EOS;
            tok->type = 1;
            return (tok->type);
         }
         else {
            state = 0;
            ungetc(ch, fp);
            tok->str[i] = EOS;
            tok->type = 1;
            return (tok->type);
         }
         break;
      case 8:
         if (ch == '=') {
            state = 0;
            tok->str[i++] = ch;
            tok->str[i] = EOS;
            tok->type = 1;
            return (tok->type);
         }
         else if (ch == '-') {
            state = 0;
            tok->str[i++] = ch;
            tok->str[i] = EOS;
            tok->type = 1;
            return (tok->type);
         }
         else if (ch == '>') {
            state = 0;
            tok->str[i++] = ch;
            tok->str[i] = EOS;
            tok->type = 1;
            return (tok->type);
         }
         else {
            state = 0;
            ungetc(ch, fp);
            tok->str[i] = EOS;
            tok->type = 1;
            return (tok->type);
         }
         break;
      case 9:
         if (ch == '=') {
            state = 0;
            tok->str[i++] = ch;
            tok->str[i] = EOS;
            tok->type = 1;
            return (tok->type);
         }
         else if (ch == '&') {
            state = 0;
            tok->str[i++] = ch;
            tok->str[i] = EOS;
            tok->type = 1;
            return (tok->type);
         }
         else {
            state = 0;
            ungetc(ch, fp);
            tok->str[i] = EOS;
            tok->type = 1;
            return (tok->type);
         }
      case 10:
         if (ch == '=') {
            state = 0;
            tok->str[i++] = ch;
            tok->str[i] = EOS;
            tok->type = 1;
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
            tok->type = 1;
            return (tok->type);
         }
         break;
      case 11:
         if (ch == '\n') {
            state = 0;
         }
         break;
      case 12:
         if (ch == '\\') {
            state = 13;
         }
         else if (ch == '"') {
            state = 0;
            tok->str[i] = EOS;
            tok->type = 5;
            return (tok->type);
         }
         else {
            tok->str[i++] = ch;
         }
         break;
      case 13:
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
      case 14:
         if (ch == '=') {
            state = 0;
            tok->str[i++] = ch;
            tok->str[i] = EOS;
            tok->type = 1;
            return (tok->type);
         }
         else {
            state = 0;
            ungetc(ch, fp);
            tok->str[i] = EOS;
            tok->type = 1;
            return (tok->type);
         }
         break;
      case 15:
         if (ch == '=') {
            state = 0;
            tok->str[i++] = ch;
            tok->str[i] = EOS;
            tok->type = 1;
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
            tok->type = 1;
            return (tok->type);
         }
         break;
      case 16:
         if (ch == '=') {
            state = 0;
            tok->str[i++] = ch;
            tok->str[i] = EOS;
            tok->type = 1;
            return (tok->type);
         }
         else {
            state = 0;
            ungetc(ch, fp);
            tok->str[i] = EOS;
            tok->type = 1;
            return (tok->type);
         }
         break;
      case 17:
         if (ch == '=') {
            state = 0;
            tok->str[i++] = ch;
            tok->str[i] = EOS;
            tok->type = 1;
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
            tok->type = 1;
            return (tok->type);
         }
         break;
      case 18:
         if (ch == '=') {
            state = 0;
            tok->str[i++] = ch;
            tok->str[i] = EOS;
            tok->type = 1;
            return (tok->type);
         }
         else {
            state = 0;
            ungetc(ch, fp);
            tok->str[i] = EOS;
            tok->type = 1;
            return (tok->type);
         }
         break;
      case 19:
         if (ch == '*') {
            state = 20;
         }
         break;
      case 20:
         if (ch == '/') {
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
            tok->value = tok->str[0];
            tok->type = 4;
            return (tok->type);
         }
         else {
            state = 23;
         }
         break;
      }
   }

   tok->type = 0;
   tok->str[0] = ch;
   tok->str[1] = EOS;

   return (tok->type);
}

void PrintToken(const struct Token *tok)
{
   switch (tok->type)
   {
   case 0:
      putc(tok->str[0], stdout);
      break;
   case 1:
      printf("TOKEN: '%s'\n", tok->str);
      break;
   case 2:
      printf("INVALID: '%c'\n", tok->str[0]);
      break;
   case 3:
      printf("NAME: '%s'\n", tok->str);
      break;
   case 4:
      printf("NUMBER: '%s' %d\n", tok->str, tok->value);
      break;
   case 5:
      printf("STRING: '%s'\n", tok->str);
      break;
   }
}


