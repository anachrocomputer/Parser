/* for --- test nested 'for' loops */

void main(void)
{
   int i;
   int j;
   
   for (i = 0; i < 4; i++)
      for (j = 0; j < 4; j++) {
         putchar('a' + i);
         putchar('\n');
      }
}

