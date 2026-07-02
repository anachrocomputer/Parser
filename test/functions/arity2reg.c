/* arity2reg --- test a void function with two parameters   2026-07-02 */

void putchar();

void Wellerman(int sugar, int tea);

void Wellerman(int sugar, int tea)
{
   register int rum;
   
   rum = 'R';
   
   putchar(sugar);
   putchar('\n');

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
