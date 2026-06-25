/* arity2 --- test a void function with two parameters      2026-06-25 */

void Wellerman(int sugar, int tea);

void Wellerman(int sugar, int tea);
{
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
