/* arity3 --- test a void function with three parameters    2026-06-25 */

void Wellerman(int sugar, int tea, int rum);

void Wellerman(int sugar, int tea, int rum);
{
   putchar(sugar);
   putchar('\n');

   putchar(tea);
   putchar('\n');

   putchar(rum);
   putchar('\n');
}


void main(void)
{
   putchar('a');  // output: a
   putchar('\n');

   Wellerman('b', 'c', 'd'); // output: b
                             // output: c
                             // output: d
   putchar('e');  // output: e
   putchar('\n');
}
