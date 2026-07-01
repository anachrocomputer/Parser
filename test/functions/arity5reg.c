/* arity5reg --- test a void function with five parameters  2026-07-01 */

void putchar();

void Wellerman(int sugar, int tea, int rum, int ship, int captain)
{
   register int wind;
   
   wind = 'N';
   
   putchar(sugar);
   putchar('\n');

   putchar(tea);
   putchar('\n');

   putchar(rum);
   putchar('\n');
   
   putchar(ship);
   putchar('\n');
   
   putchar(captain);
   putchar('\n');
}


void main(void)
{
   putchar('a');  // output: a
   putchar('\n');

   Wellerman('b', 'c', 'd', 'e', 'f'); // output: b
                                       // output: c
                                       // output: d
                                       // output: e
                                       // output: f
   putchar('g');  // output: g
   putchar('\n');
}
