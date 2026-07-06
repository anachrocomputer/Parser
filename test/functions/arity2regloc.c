/* arity2regloc --- void function with local variables      2026-07-07 */

void putchar(const int c);

void Wellerman(int sugar, int tea);

void Wellerman(int sugar, int tea)
{
   register int rum;
   int whale;
   
   rum = 'R';	// Arrr
   whale = 42;
   
   putchar(sugar);
   putchar('\n');
   
   whale = 'R';
   rum = 42;
   
   putchar(tea);
   putchar('\n');
}


void main(void)
{
   putchar('a');  // output: a
   putchar('\n');

   Wellerman('b', 'c'); // output: b
                        // output: c
   putchar('d');  // output: d
   putchar('\n');
}
