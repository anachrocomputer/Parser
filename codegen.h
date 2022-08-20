/* codegen --- code generation routines                     2022-08-12 */
/* Copyright (c) 2022 John Honniball. All rights reserved              */

void CodeGenInit(void);
int OpenAssemblerFile(const char fname[]);
int CloseAssemblerFile(void);
int Emit(const char inst[], const char oper[], const char comment[]);
int AllocLabel(const char purpose);
void EmitLabel(const int label);
int EmitStaticLong(const int label, const long int init, const char comment[]);
int LoadStaticLong(const int label, const char comment[]);
int EmitStaticInt(const int label, const int init, const char comment[]);
int LoadIntConstant(const int val, const int reg, const char comment[]);
int EmitExternInt(const char name[], const int init, const char comment[]);
int LoadExternInt(const char name[], const int reg, const char comment[]);
int StoreExternInt(const char name[], const int reg, const char comment[]);
int LoadStaticInt(const int label, const int reg, const char comment[]);
int StoreStaticInt(const int label, const int reg, const char comment[]);
int EmitStaticChar(const int label, const int init, const char comment[]);
int LoadStaticChar(const int label, const int reg, const bool signExtend, const char comment[]);
int EmitJump(const int label, const char comment[]);
