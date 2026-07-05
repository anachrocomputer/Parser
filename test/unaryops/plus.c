/* plus --- test unary plus                                 2026-07-04 */

void puti(const int i);

int Sugar = 4200;

void main(void)
{
   int tea;
   register int rum;
   
   tea = +42;
   rum = -42;
   
   puti(+0);      // output: 0

   puti(+1);      // output: 1
   
   puti(+(+1));   // output: 1

   puti(+32767);  // output: 32767
   
   puti(+tea);    // output: 42
   
   puti(+rum);     // output: -42
   
   puti(+rum);    // output: -42
   
   puti(+(+rum)); // output: -42
   
   puti(+Sugar);  // output: 4200
   
   Sugar = +Sugar;
   
   puti(Sugar);   // output: 4200
}
