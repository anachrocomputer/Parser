/* bitwisenot --- test binary bitwise inversion             2026-07-03 */

void puti(const int i);
void putu(const int u);

int Sugar = 0xffff;

void main(void)
{
   int tea;
   register int rum;
   
   tea = 0x5555;
   rum = ~0xaaaa;
   
   puti(~0);      // output: -1

   putu(~0);      // output: 65535

   puti(~32767);  // output: -32768
   
   putu(~tea);    // output: 43690
   
   putu(rum);     // output: 21845
   
   putu(~rum);    // output: 43690
   
   puti(~Sugar);  // output: 0
   
   Sugar = ~Sugar;
   
   puti(Sugar);   // output: 0
   
   putu(Sugar);   // output: 0
   
   puti(~Sugar);   // output: -1
   
   putu(~Sugar);   // output: 65535
}
