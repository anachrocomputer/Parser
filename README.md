# Simple Lexical Analyser and Parser #

Currently able to tokenise most of the C language and parse very limited
external declarations, functions, and statements.

See 'simpledecl.c' and 'simplectrl.c' for example files that can be parsed.

Use the '-T' command-line option to enable token-level debugging output.
Use the '-S' command-line option to enable syntax trace output.

## Code Generation ##

Code generation for the 6809 and 6309 using the 'asm6809' assembler.

TODO: add a '-PIC' command-line option for position-independent code.
I think the 6809 architecture is suitable for that.
