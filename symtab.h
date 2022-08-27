/* symtab --- symbol table routines                         2022-08-27 */
/* Copyright (c) 2022 John Honniball. All rights reserved              */

void SymTabInit(void);
void AddExternSymbol(const char name[], int type, int pLevel);
int LookUpExternSymbol(const char name[]);
