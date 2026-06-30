/* intfunc --- test nested int function calls               2026-06-30 */

void putchar();

int NameOfTheShip(void)
{
   putchar('2');
   putchar('\n');
   
   return ('3');
}

void PutToSea(int rum)
{
   putchar(rum);
   putchar('\n');
}


void main(void)
{
   putchar('1');  // output: 1
   putchar('\n');
   
   PutToSea(NameOfTheShip()); // output: 2
                              // output: 3

   putchar('4');  // output: 4
   putchar('\n');
}

