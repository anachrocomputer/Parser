/* arity1 --- test a void function with one parameter       2026-06-25 */

void Wellerman(int sugar);

void Wellerman(int sugar)
{
   putchar(sugar);
   putchar('\n');
}


void main(void)
{
   putchar('a');  // output: a
   putchar('\n');

   Wellerman('b'); // output: b

   putchar('c');  // output: c
   putchar('\n');
}
