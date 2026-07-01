/* ifelse --- test an empty if-else statement               2026-07-01 */

void putchar();

void main(void)
{
   int rum;

   putchar('a');  // output: a
   putchar('\n');
   
   rum = 4;
   
   if (rum)
      ;
   else
      ;

   putchar('b');  // output: b
   putchar('\n');
}

