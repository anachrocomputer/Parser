/* void --- test a void function with no parameters         2026-06-25 */

void putchar();

void PutToSea(void);

void PutToSea(void)
{
   putchar('b');
   putchar('\n');
}


void main(void)
{
   putchar('a');    // output: a
   putchar('\n');

   PutToSea();      // output: b

   putchar('c');    // output: c
   putchar('\n');
}
