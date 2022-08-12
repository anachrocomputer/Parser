/* codegen --- code generation routines                     2022-08-12 */

int OpenAssemblerFile(const char fname[]);
int CloseAssemblerFile(void);
int Emit(const char inst[], const char oper[], const char comment[]);
int AllocLabel(const char purpose);
void EmitLabel(const int label);
int EmitStaticLong(const int label, const long int init, const char comment[]);
int EmitStaticInt(const int label, const int init, const char comment[]);
int EmitStaticChar(const int label, const int init, const char comment[]);
int EmitJump(const int label, const char comment[]);
