/* arity1reg --- test a void function with one parameter    2026-07-02 */

void putchar();

void Wellerman(int sugar);

void Wellerman(int sugar)
{
   register int tea;
   
   tea = 't';
   
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
