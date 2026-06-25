/* void --- test a void function with no parameters         2026-06-25 */

void PutToSea(void);

void PutToSea(void)
{
   putchar('b');
   putchar('\n');
}


void main(void)
{
   putchar('a');
   putchar('\n');

   PutToSea();

   putchar('c');
   putchar('\n');
}
