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
void EmitFunctionEntry(const char name[], const int nBytes);
void EmitFunctionExit(const int returnLabel);
void EmitStaticCharArray(const struct StringConstant *sc, const char name[]);
void EmitStaticLong(const int label, const long int init, const char comment[]);
void LoadScalar(const struct Symbol *sym, char comment[]);
void StoreScalar(const struct Symbol *sym, char comment[]);
void EmitStaticInt(const int label, const int init, const char comment[]);
void EmitStaticFloat(const int label, const float init, const char comment[]);
void EmitStaticDouble(const int label, const double init, const char comment[]);
void LoadIntConstant(const int val, const int reg, const char comment[]);
void LoadLabelAddr(const int label, const char comment[]);
void EmitExternChar(const char name[], const int init, const char comment[]);
void EmitExternInt(const char name[], const int init, const char comment[]);
void EmitExternPointer(const char name[], const int init, const char comment[]);
void EmitExternFloat(const char name[], const float init, const char comment[]);
void EmitExternDouble(const char name[], const double init, const char comment[]);
void EmitStaticChar(const int label, const int init, const char comment[]);
void EmitJump(const int label, const char comment[]);
void EmitBranchIfEqual(const int label, const char comment[]);
void EmitBranchNotEqual(const int label, const char comment[]);
void EmitIncExternInt(const char name[], const int amount);
void EmitCompareIntConstant(const int compare, const char comment[]);
void EmitCallFunction(const char name[], const char comment[]);
