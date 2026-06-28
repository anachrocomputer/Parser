/* unsigned --- test unsigned integer literals              2026-06-28 */

void putu();

void main(void)
{
   putu(0u);  // output: 0

   putu(00u);  // output: 0

   putu(32767u);  // output: 32767

   putu(0x0u);  // output: 0

   putu(0x00u);  // output: 0

   putu(0xffffu);  // output: 65535

   putu(0xFFFFu);  // output: 65535

   putu(0Xffffu);  // output: 65535

   putu(0XFFFFu);  // output: 65535

   putu(0177777u);  // output: 65535

   putu(0xFFu);    // output: 255

   putu(0xffu);    // output: 255

   putu(0377u);    // output: 255
}
