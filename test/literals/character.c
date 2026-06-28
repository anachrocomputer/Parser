/* character --- character literals                         2026-06-28 */

void putchar();

void main(void)
{
   putchar('a');  // output: a
   putchar('\n');

   putchar('@');  // output: @
   putchar('\n');

   putchar('`');  // output: `
   putchar('\n');

   putchar('$');  // output: $
   putchar('\n');

   putchar('~');  // output: ~
   putchar('\n');

   putchar('_');  // output: _
   putchar('\n');

   putchar('0');  // output: 0
   putchar('\n');

   putchar('9');  // output: 9
   putchar('\n');
   // TODO \n \t \a \012 \x0d \xff, etc. But how to code the expected output?
}
