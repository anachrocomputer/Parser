/* if --- test nested 'if' statements                       2026-06-23 */

void putchar(int);

void BillyOTea(int ship, int whale)
{
   if (ship)
      if (whale)
         putchar('d');
      else
         putchar('c');
   else
      if (whale)
         putchar('b');
      else
         putchar('a');
         
   putchar('\n');
}

void main(void)
{
   BillyOTea(0, 0);  // output: a
   BillyOTea(0, 1);  // output: b
   BillyOTea(1, 0);  // output: c
   BillyOTea(1, 1);  // output: d
}

