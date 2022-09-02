/* lexical --- lexical analyser module                      2022-08-15 */
/* Copyright (c) 2022 John Honniball. All rights reserved              */

enum eToken {
   TNULL, TINVAL, TSEMI, TCOLON, TCOMMA, TQUEST,
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
   TEOF,
};


struct Token {
   char sValue[256];
   int sLength;
   int/**/iValue;
   double/*/*/fValue;
   enum/*//*/eToken/***/token;
   char/* */str[256];
};

void LexicalInit(void);
void SetTokenTraceFlag(const bool enabled);
void SetSyntaxTraceFlag(const bool enabled);
void PrintSyntax(const char *fmt, ...) __attribute__ ((format (printf, 1, 2)));
void Error(const char *fmt, ...) __attribute__ ((format (printf, 1, 2)));
void installkw(const char keyword[], enum eToken token, bool isType);
bool OpenSourceFile(const char fname[]);
bool CloseSourceFile(void);
int GetToken(struct Token *tok);
char *tokenAsStr(const struct Token *tok);
enum eToken lookupKeyword(const char name[]);
void PrintToken(const struct Token *tok);
