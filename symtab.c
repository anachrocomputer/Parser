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

bool AddExternSymbol(const struct Symbol *sym)
{
   int i;
   
   for (i = 0; i < NextSym; i++) {
      if (strcmp(SymTab[i].name, sym->name) == 0) {
         return (false);
      }
   }

   SymTab[NextSym].storageClass = sym->storageClass;
   strncpy(SymTab[NextSym].name, sym->name, MAXNAME);
   SymTab[NextSym].type = sym->type;
   SymTab[NextSym].pLevel = sym->pLevel;
   SymTab[NextSym].label = sym->label;
   SymTab[NextSym].fpOffset = sym->fpOffset;
   
   NextSym++;
   
   return (true);
}


/* LookUpExternSymbol --- look for a symbol in the table of 'extern's */

struct Symbol *LookUpExternSymbol(const char name[])
{
   int i;
   
   for (i = 0; i < NextSym; i++) {
      if (strcmp(SymTab[i].name, name) == 0) {
         return (&SymTab[i]);
      }
   }

   return (NULL);
}


