/* integer --- verify integer literals                      2026-06-28 */

void putl();

void main(void)
{
   putl(0L);  // output: 0

   putl(00L);  // output: 0

   putl(32767L);  // output: 32767

   putl(0x0L);  // output: 0

   putl(0x00L);  // output: 0

   putl(0x7fffL);  // output: 32767

   putl(0x7FFFL);  // output: 32767

   putl(0X7fffL);  // output: 32767

   putl(0X7FFFL);  // output: 32767

   putl(077777L);  // output: 32767

   putl(0xFFL);    // output: 255

   putl(0xffL);    // output: 255

   putl(0377L);    // output: 255
}
