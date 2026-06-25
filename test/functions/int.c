/* int --- test a function returning int with no parameters 2026-06-25 */

int PutToSea(void);

int PutToSea(void)
{
   putchar('b');
   putchar('\n');
   
   return ('c');
}


void main(void)
{
   int rum;
   
   putchar('a');  // output: a
   putchar('\n');

   rum = PutToSea(); // output: b

   putchar(rum);  // output: c
   putchar('\n');
}

