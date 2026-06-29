/* switch --- test a really big 'switch' statement */

void putchar();

void main(void)
{
   int ship;

   ship = 6;
   
   switch (ship) {
   case 0:
      putchar('a');
      break;
   case 1:
      putchar('a');
      break;
   case 2:
      putchar('b');
      break;
   case 3:
      putchar('c');
      break;
   case 4:
      putchar('d');
      break;
   case 5:
      putchar('e');
      break;
   case 6:
      putchar('f');  // output: f
      break;
   case 7:
      putchar('g');
      break;
   }
   
   putchar('\n');
}

