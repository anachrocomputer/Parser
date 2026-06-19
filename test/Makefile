# Makefile for test suite based on the 6809 simulator

# We build the hex file with Ciaran Anscomb's 6309-capable assembler from 6809.org.uk. 
 
AS63=asm6809 
AS63FLAGS=--6309 

CC=../parser
CFLAGS=

SIM6809=sim6809
SIMFLAGS=-n -g -q

#all: assignment/local.out assignment/global.out assignment/module.out assignment/static.out \
#     assignment/register.out
all: assignment/local.out assignment/global.out assignment/static.out

assignment/local.out: assignment/local.hex
	$(SIM6809) $(SIMFLAGS) assignment/local.hex >assignment/local.out

assignment/local.hex: assignment/local.asm
	$(AS63) $(AS63FLAGS) -H -o assignment/local.hex -l assignment/local.lst assignment/local.asm

assignment/local.asm: assignment/local.c $(CC)
	$(CC) $(CFLAGS) assignment/local.c

assignment/global.out: assignment/global.hex
	$(SIM6809) $(SIMFLAGS) assignment/global.hex >assignment/global.out

assignment/global.hex: assignment/global.asm
	$(AS63) $(AS63FLAGS) -H -o assignment/global.hex -l assignment/global.lst assignment/global.asm

assignment/global.asm: assignment/global.c $(CC)
	$(CC) $(CFLAGS) assignment/global.c

assignment/module.out: assignment/module.hex
	$(SIM6809) $(SIMFLAGS) assignment/module.hex >assignment/module.out

assignment/module.hex: assignment/module.asm
	$(AS63) $(AS63FLAGS) -H -o assignment/module.hex -l assignment/module.lst assignment/module.asm

assignment/module.asm: assignment/module.c $(CC)
	$(CC) $(CFLAGS) assignment/module.c

assignment/static.out: assignment/static.hex
	$(SIM6809) $(SIMFLAGS) assignment/static.hex >assignment/static.out

assignment/static.hex: assignment/static.asm
	$(AS63) $(AS63FLAGS) -H -o assignment/static.hex -l assignment/static.lst assignment/static.asm

assignment/static.asm: assignment/static.c $(CC)
	$(CC) $(CFLAGS) assignment/static.c

