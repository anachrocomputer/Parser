/* forreg --- test nested 'for' loops with register         2026-07-01 */

void putchar();

void main(void)
{
   int i;
   register int j;
   int rum;
   
   for (i = 4; i ; i--) {
      rum = 'R';
      
      for (j = 4; j ; j--)
         putchar(rum++);
      
      putchar('\n');
   }
   // output: RSTU
   // output: RSTU
   // output: RSTU
   // output: RSTU
}

