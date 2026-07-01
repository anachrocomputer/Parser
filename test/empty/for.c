/* for --- test an empty for-loop                           2026-07-01 */

void putchar();

void main(void)
{
   int rum;

   putchar('a');  // output: a
   putchar('\n');
   
   for (rum = 4; rum ; rum--)
      ;

   putchar('b');  // output: b
   putchar('\n');
}

