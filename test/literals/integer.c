/* integer --- verify integer literals                      2026-06-28 */

void puti(const int i);

void main(void)
{
   puti(0);       // output: 0

   puti(00);      // output: 0

   puti(100);     // output: 100
   
   puti(1000);    // output: 1000
   
   puti(10000);   // output: 10000
   
   puti(32767);   // output: 32767

   puti(0x0);     // output: 0

   puti(0x00);    // output: 0

   puti(0x7fff);  // output: 32767

   puti(0x7FFF);  // output: 32767

   puti(0X7fff);  // output: 32767

   puti(0X7FFF);  // output: 32767

   puti(077777);  // output: 32767

   puti(0xFF);    // output: 255

   puti(0xff);    // output: 255

   puti(0377);    // output: 255
   
   puti(-1);      // output: -1
   
   puti(-100);    // output: -100
   
   puti(-1000);   // output: -1000
   
   puti(-10000);  // output: -10000
   
   puti(-255);    // output: -255
   
   puti(-0377);   // output: -255
   
   puti(-32767);  // output: -32767
   
   puti(-0x7fff); // output: -32767
}
