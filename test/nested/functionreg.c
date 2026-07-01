/* functionreg --- test nested void function calls with register 2026-07-01 */

void putchar();

void NameOfTheShip(void)
{
   register int ship;
   
   ship = '3';
   
   putchar(ship);
   putchar('\n');
}

void PutToSea(void)
{
   register int sea;
   
   sea = '4';
   
   putchar('2');
   putchar('\n');

   NameOfTheShip();

   putchar(sea);
   putchar('\n');
}


void main(void)
{
   register int captain;
   
   captain = '5';
   
   putchar('1');  // output: 1
   putchar('\n');
   
   PutToSea();    // output: 2
                  // output: 3
                  // output: 4

   putchar(captain);  // output: 5
   putchar('\n');
}

