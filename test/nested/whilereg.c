/* whilereg --- test nested 'while' loops with register     2026-07-01 */

void putchar();

void main(void)
{
   int sugar;
   register int tea;
   int rum;
   
   rum = 8;
   while (rum) {
      sugar = 'A';

      tea = 8;
      while (tea) {
         putchar(sugar++); // output: ABCDEFGH
                           // output: ABCDEFGH
                           // output: ABCDEFGH
                           // output: ABCDEFGH
                           // output: ABCDEFGH
                           // output: ABCDEFGH
                           // output: ABCDEFGH
                           // output: ABCDEFGH
         tea--;
      }

      putchar('\n');
      rum--;
   }
}
