/* arity4 --- test a void function with four parameters     2026-06-30 */

void putchar();

void Wellerman(int sugar, int tea, int rum, int ship)
{
   putchar(sugar);
   putchar('\n');

   putchar(tea);
   putchar('\n');

   putchar(rum);
   putchar('\n');
   
   putchar(ship);
   putchar('\n');
}


void main(void)
{
   putchar('a');  // output: a
   putchar('\n');

   Wellerman('b', 'c', 'd', 'e'); // output: b
                                  // output: c
                                  // output: d
                                  // output: e
   putchar('f');  // output: f
   putchar('\n');
}
