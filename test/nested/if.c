/* if --- test nested 'if' statements */

void main(void)
{
   int ship = 0;
   int whale = 1;
   
   if (ship)
      if (whale)
         putchar('a');  // output: a
      else
         putchar('b');
   else
      if (whale)
         putchar('c');
      else
         putchar('d');
         
   putchar('\n');
}

