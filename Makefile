# Makefile for C parser                                        2022-08-04

CC=gcc
CFLAGS=-c

LD=gcc
LDFLAGS=

AS=asm6809
ASFLAGS=--6309

all: parser ex1.hex ex1.srec

parser.o: parser.c codegen.h
	$(CC) $(CFLAGS) -o parser.o parser.c

codegen.o: codegen.c codegen.h
	$(CC) $(CFLAGS) -o codegen.o codegen.c

parser: parser.o codegen.o
	$(LD) $(LDFLAGS) -o parser parser.o codegen.o

ex1.hex: ex1.asm
	$(AS) $(ASFLAGS) -H -o ex1.hex -l ex1.lst ex1.asm

ex1.srec: ex1.asm
	$(AS) $(ASFLAGS) -S -o ex1.srec -l ex1.lst ex1.asm
