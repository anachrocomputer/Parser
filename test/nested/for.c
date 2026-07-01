/* for --- test nested 'for' loops                          2026-06-29 */

void putchar();

void main(void)
{
   int i;
   int j;
   int rum;
   
   for (i = 4; i ; i--) {
      rum = 'A';
      
      for (j = 4; j ; j--)
         putchar(rum++);
      
      putchar('\n');
   }
   // output: ABCD
   // output: ABCD
   // output: ABCD
   // output: ABCD
}

