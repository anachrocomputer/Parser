/* switch --- test nested 'switch' statements */

void main(void)
{
   int i = 1;
   int j = 1;
   
   switch (i) {
   case 0:
      switch (j) {
      case 0:
         putchar('a');
         putchar('\n');
         break;
      case 1:
         putchar('b');
         putchar('\n');
         break;
      }
      break;
   case 1:
      switch (j) {
      case 0:
         putchar('c');
         putchar('\n');
         break;
      case 1:
         putchar('d');  // output: d
         putchar('\n');
         break;
      }
      break;
   }
}

