/* switch --- test nested 'switch' statements               2026-06-23 */

void Ship(int sugar, int tea)
{
   switch (sugar) {
   case 0:
      switch (tea) {
      case 0:
         putchar('a');
         break;
      case 1:
         putchar('b');
         break;
      }
      break;
   case 1:
      switch (j) {
      case 0:
         putchar('c');
         break;
      case 1:
         putchar('d');
         break;
      }
      break;
   }

   putchar('\n');
}

void main(void)
{
   Ship(0, 0);    // output: a
   Ship(0, 1);    // output: b
   Ship(1, 0);    // output: c
   Ship(1, 1);    // output: d
}

