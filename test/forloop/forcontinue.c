/* forcontinue --- test 'for' loop with 'continue'          2026-07-05 */

void putchar(const char c);
void puti();

void main(void)
{
   int i;
   int rum;
   int tea;
   
   rum = '@';
   tea = 5;
   
   for (i = 10; i ; i--) {
      tea--;
      rum++;
      
      if (!tea)
         continue;
      
      putchar(rum);
      putchar(' ');
      puti(i);
   }
   
   // output: A 10
   // output: B 9
   // output: C 8
   // output: D 7

   // output: F 5
   // output: G 4
   // output: H 3
   // output: I 2
   // output: J 1
}

