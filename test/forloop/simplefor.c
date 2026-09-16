/* simplefor --- test simple 'for' loops                    2026-07-05 */

void putchar(const char ch);

void main(void)
{
   int i;
   int rum;
   
   rum = 'A';
   
   for (i = 6; i ; i--)
      putchar(rum++);
      
   putchar('\n');
   // output: ABCDEF
}

