/* symtab --- symbol table routines                         2022-08-27 */
/* Copyright (c) 2022 John Honniball. All rights reserved              */

#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#include "symtab.h"

#define MAXSYMS   (256)

static int NextSym = 0;
static struct Symbol SymTab[MAXSYMS];
static int NextLocalSym = 0;
static struct Symbol LocalSymTab[MAXSYMS];


/* SymTabInit --- initialise this module */

void SymTabInit(void)
{
}


/* AddExternSymbol --- add a symbol to the table of 'extern's */

bool AddExternSymbol(const struct Symbol *const sym)
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
   SymTab[NextSym].readOnly = sym->readOnly;
   
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


/* AddLocalSymbol --- add a symbol to the table of local variables */

bool AddLocalSymbol(const struct Symbol *const sym)
{
   int i;
   
   for (i = 0; i < NextLocalSym; i++) {
      if (strcmp(LocalSymTab[i].name, sym->name) == 0) {
         return (false);
      }
   }

   LocalSymTab[NextLocalSym].storageClass = sym->storageClass;
   strncpy(LocalSymTab[NextLocalSym].name, sym->name, MAXNAME);
   LocalSymTab[NextLocalSym].type = sym->type;
   LocalSymTab[NextLocalSym].pLevel = sym->pLevel;
   LocalSymTab[NextLocalSym].label = sym->label;
   LocalSymTab[NextLocalSym].fpOffset = sym->fpOffset;
   LocalSymTab[NextLocalSym].readOnly = sym->readOnly;
   
   NextLocalSym++;
   
   return (true);
}


/* LookUpLocalSymbol --- look for a symbol in the table of local variables */

struct Symbol *LookUpLocalSymbol(const char name[])
{
   int i;
   
   for (i = 0; i < NextLocalSym; i++) {
      if (strcmp(LocalSymTab[i].name, name) == 0) {
         return (&LocalSymTab[i]);
      }
   }

   return (NULL);
}


/* ForgetLocalSymbols --- clear the local symbol table */

void ForgetLocalSymbols(void)
{
   NextLocalSym = 0;
}

