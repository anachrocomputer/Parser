/* symtab --- symbol table routines                         2022-08-27 */
/* Copyright (c) 2022 John Honniball. All rights reserved              */

#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#include "symtab.h"

#define MAXSYMS   (256)

static int NextSym = 0;
static struct Symbol SymTab[MAXSYMS];


/* SymTabInit --- initialise this module */

void SymTabInit(void)
{
}


/* AddExternSymbol --- add a symbol to the table of 'extern's */

void AddExternSymbol(const char name[], int type, int pLevel)
{
   strncpy(SymTab[NextSym].name, name, MAXNAME);
   SymTab[NextSym].type = type;
   SymTab[NextSym].pLevel = pLevel;
   
   NextSym++;
}


/* LookUpExternSymbol --- look for a symbol in the table of 'extern's */

int LookUpExternSymbol(const char name[])
{
   return (0);
}


