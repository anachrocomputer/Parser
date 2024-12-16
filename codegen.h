/* codegen --- code generation routines                     2022-08-12 */
/* Copyright (c) 2022 John Honniball. All rights reserved              */

#include "symtab.h"

#define NOLABEL    (-1)

struct StringConstant {
   int label;
   char str[256];
   char sValue[256];
   int sLength;
};

void CodeGenInit(void);
bool OpenAssemblerFile(const char fname[]);
bool CloseAssemblerFile(void);
int Emit(const char inst[], const char oper[], const char comment[]);
int AllocLabel(const char purpose);
void EmitLabel(const int label);
void EmitFunctionEntry(const char name[], const int nBytes, const int nRegister);
void EmitFunctionExit(const int returnLabel, const int nRegister);
void EmitStackCleanup(const int nBytes);
void EmitStaticCharArray(const struct StringConstant *sc, const char name[]);
void LoadScalar(const struct Symbol *const sym);
void StoreScalar(const struct Symbol *const sym);
void LoadIntConstant(const int val, const int reg, const char comment[]);
void LoadLabelAddr(const int label, const char comment[]);
void EmitExternScalar(const struct Symbol *const sym, const int init, const double fInit);
void EmitJump(const int label, const char comment[]);
void EmitBranchIfEqual(const int label, const char comment[]);
void EmitBranchNotEqual(const int label, const char comment[]);
void EmitIncScalar(const struct Symbol *const sym, const int amount);
void EmitCompareIntConstant(const int compare, const char comment[]);
void EmitCallFunction(const char name[], const char comment[]);
