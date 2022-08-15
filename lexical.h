/* lexical --- lexical analyser module                      2022-08-15 */
/* Copyright (c) 2022 John Honniball. All rights reserved              */

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

void LexicalInit(void);
int GetToken(FILE *fp, struct Token *tok);
char *tokenAsStr(const struct Token *tok);
void installkw(const char keyword[], enum eToken token, bool isType);
enum eToken lookupKeyword(const char name[]);
void PrintToken(const struct Token *tok);
