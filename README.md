# Simple Lexical Analyser and Parser #

Currently able to tokenise most of the C language and parse very limited
external declarations, functions, and statements.

## Example Code ##

All the examples were inspired by the Wellerman sea shanty.

See 'simpledecl.c' and 'simplectrl.c' for example files that can be parsed.
'simplefunc.c' is an example file that can be compiled, assembled,
transferred to a 6809 machine and run (it relies on I/O via the monitor ROM
routines of my UK109, but can be modified for other hardware).

## Compiler Tests ##

In the subdirectory 'test' there is a Python script that will execute
a number of test programs and check whether they pass or fail.
The test results are written in HTML format into a file called 'results.html'.
The test runner aims to be robust against gross failures in the compiler such
as endless error messages and SEGFAULTs.
Errors at the assembler stage are also checked for.

The resulting 6809 programs (in Intel HEX format) are executed by a 6809
simulator.
The output from the simulator is captured by the Python script (via a Unix pipe)
and compared with the expected output.
In the C source code for the tests,
specially-formatted one-line comments are used to describe the expected output.
At present,
that only includes run-time output but in future will be expanded to specify
compile-time error messages.
When that improvement has been made,
tests will be added to generate compile-time errors and check them.

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

## Inspiration ##

I've read a number of compiler books over the years,
including P. J. Brown's *Writing Interactive Compilers and Interpreters* (1979)
and more recently Robert Nystrom's *Crafting Interpreters* (2021).
I also have *Modern Compiler Implementation In C* by Andrew W. Appel and have recently
acquired *Writing A C Compiler* by Nora Sandler.
Nystrom's and Sandler's books have inspired the compiler tests and the script to run them.
The book that originally motivated me to begin writing the compiler is *Understanding
and Writing Compilers* (1979) by Richard Bornat.
