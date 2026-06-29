/* while --- test nested 'while' loops                      2026-06-25 */

void putchar();

void main(void)
{
   int sugar;
   int tea;
   int rum;
   
   rum = 10;
   while (rum) {
      sugar = 'A';

      tea = 10;
      while (tea) {
         putchar(sugar++); // output: ABCDEFGHIJ
         tea--;
      }

      putchar('\n');
      rum--;
   }
}
