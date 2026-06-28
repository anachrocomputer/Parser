/* integer --- verify integer literals                      2026-06-28 */

void puti();

void main(void)
{
   puti(0);  // output: 0

   puti(00);  // output: 0

   puti(32767);  // output: 32767

   puti(0x0);  // output: 0

   puti(0x00);  // output: 0

   puti(0x7fff);  // output: 32767

   puti(0x7FFF);  // output: 32767

   puti(0X7fff);  // output: 32767

   puti(0X7FFF);  // output: 32767

   puti(077777);  // output: 32767

   puti(0xFF);    // output: 255

   puti(0xff);    // output: 255

   puti(0377);    // output: 255
}
