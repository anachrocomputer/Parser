/* forbreak --- test 'for' loop with 'break'                2026-07-05 */

void putchar();

void main(void)
{
   int i;
   int rum;
   int tea;
   
   rum = 'A';
   tea = 6;
   
   for (i = 10; i ; i--) {
      putchar(rum++);
      
      tea--;
      
      if (!tea)
         break;
   }
      
   putchar('\n');
   // output: ABCDEF
}

