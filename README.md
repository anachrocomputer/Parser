# Simple Lexical Analyser and Parser #

Currently able to tokenise most of the C language and parse very limited
external declarations, functions, and statements.

## Example Code ##

All the examples were inspired by the Wellerman sea shanty.

See 'simpledecl.c' and 'simplectrl.c' for example files that can be parsed.
'simplefunc.c' is an example file that can be compiled, assembled,
transferred to a 6809 machine and run (it relies on I/O via the monitor ROM
routines of my UK109, but can be modified for other hardware).

## Command-Line Arguments ##

Use the '-T' command-line option to enable token-level debugging output.
Use the '-S' command-line option to enable syntax trace output.

## C Language Standard ##

I'm aiming for ANSI C (C89) but also recognising some more recent keywords.
I do not intend to support trigraphs.
The **register** keyword does allow a single 16-bit variable to be placed in the Y register.
**inline** will have no effect.

There's no preprocessor yet,
so no include files, no conditional compilation, and no \#defined names.

## Code Generation ##

Code generation for the 6809 and 6309 using the 'asm6809' assembler.

There is no facility as yet for separate compilation units and/or a linker.

TODO: add a '-PIC' command-line option for position-independent code.
I think the 6809 architecture is suitable for that.

## Run-Time Library ##

Some very primitive I/O routines are available so far,
just by adding them to the run-time startup code generated in every output file.
I may add some string functions once I have parameter passing working.

