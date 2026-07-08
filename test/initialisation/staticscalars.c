/* staticscalars --- test initialisation of static scalars  2026-07-08 */

void puti(const int i);
void putu(const int u);
void putchar(const char c);

void main(void)
{
   static char ship = 'S' + 1;
   static int sailors = 7 * 6;
   static float soon = 12.0f;
   static double cargo = 153e4;
   static float whale = 12.3e4f;
   static double rum = 0.3e-2;
   
   putchar(ship);    // output: T
   putchar('\n');
   
   puti(sailors);    // output: 42
}

