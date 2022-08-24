/* codegen --- code generation routines                     2022-08-12 */
/* Copyright (c) 2022 John Honniball. All rights reserved              */

void CodeGenInit(void);
bool OpenAssemblerFile(const char fname[]);
bool CloseAssemblerFile(void);
int Emit(const char inst[], const char oper[], const char comment[]);
int AllocLabel(const char purpose);
void EmitLabel(const int label);
void EmitFunctionLabel(const char name[]);
void EmitStaticLong(const int label, const long int init, const char comment[]);
void LoadStaticLong(const int label, const char comment[]);
void EmitStaticInt(const int label, const int init, const char comment[]);
void LoadIntConstant(const int val, const int reg, const char comment[]);
void EmitExternChar(const char name[], const int init, const char comment[]);
void EmitExternInt(const char name[], const int init, const char comment[]);
void EmitExternPointer(const char name[], const int init, const char comment[]);
void EmitExternFloat(const char name[], const float init, const char comment[]);
void EmitExternDouble(const char name[], const double init, const char comment[]);
void LoadExternInt(const char name[], const int reg, const char comment[]);
void StoreExternInt(const char name[], const int reg, const char comment[]);
void LoadStaticInt(const int label, const int reg, const char comment[]);
void StoreStaticInt(const int label, const int reg, const char comment[]);
void EmitStaticChar(const int label, const int init, const char comment[]);
void LoadStaticChar(const int label, const int reg, const bool signExtend, const char comment[]);
void EmitJump(const int label, const char comment[]);
void EmitBranchIfEqual(const int label, const char comment[]);
