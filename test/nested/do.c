/* do --- test nested 'do' loops                            2026-07-01 */

void putchar();

void main(void)
{
   int sugar;
   int tea;
   int rum;
   
   rum = 10;
   do {
      sugar = 'A';

      tea = 10;
      do {
         putchar(sugar++); // output: ABCDEFGHIJ
                           // output: ABCDEFGHIJ
                           // output: ABCDEFGHIJ
                           // output: ABCDEFGHIJ
                           // output: ABCDEFGHIJ
                           // output: ABCDEFGHIJ
                           // output: ABCDEFGHIJ
                           // output: ABCDEFGHIJ
                           // output: ABCDEFGHIJ
                           // output: ABCDEFGHIJ
         tea--;
      } while (tea);

      putchar('\n');
      rum--;
   } while (rum);
}
