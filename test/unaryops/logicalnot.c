/* logicalnot --- test logical inversion                    2026-07-04 */

void puti(const int i);
void putu(const int u);

int Sugar = 0xffff;

void main(void)
{
   int tea;
   register int rum;
   
   tea = 0x5555;
   rum = !0xaaaa;
   
   puti(!0);      // output: 1

   putu(!1);      // output: 0

   putu(!!0);     // output: 0
   
   puti(!!1);     // output: 1
   
   putu(!tea);    // output: 0
   
   putu(rum);     // output: 0
   
   putu(!rum);    // output: 1
   
   puti(!Sugar);  // output: 0
   
   Sugar = !Sugar;
   
   puti(Sugar);   // output: 0
   
   puti(!Sugar);   // output: 1
   
   puti(!!Sugar);  // output: 0
}
