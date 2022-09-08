/* symtab --- symbol table routines                         2022-08-27 */
/* Copyright (c) 2022 John Honniball. All rights reserved              */

#define MAXNAME   (32)

enum eStorageClass {SCAUTO, SCEXTERN, SCREGISTER, SCSTATIC};
enum eType {T_CHAR, T_UCHAR, T_SHORT, T_USHORT, T_INT, T_UINT,
            T_LONG, T_ULONG, T_FLOAT, T_DOUBLE, T_VOID};

struct Symbol {
   int storageClass;
   char name[MAXNAME];
   int type;
   int pLevel;
   int label;
   int fpOffset;
};

void SymTabInit(void);
void AddExternSymbol(const char name[], int type, int pLevel);
int LookUpExternSymbol(const char name[]);
