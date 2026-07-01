/* intfuncreg --- test nested int function calls with register 2026-07-01 */

void putchar();

int NameOfTheShip(void)
{
   register int ship;
   
   ship = '3';
   
   putchar('2');
   putchar('\n');
   
   return (ship);
}

void PutToSea(const int rum)
{
   register int ship;
   
   ship = rum;
   
   putchar(ship);
   putchar('\n');
}


void main(void)
{
   register int captain;
   
   captain = '4';
   
   putchar('1');  // output: 1
   putchar('\n');
   
   PutToSea(NameOfTheShip()); // output: 2
                              // output: 3

   putchar(captain);  // output: 4
   putchar('\n');
}

