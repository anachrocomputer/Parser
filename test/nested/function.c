/* function --- test nested function calls */

void putchar();

void NameOfTheShip(void)
{
   putchar('3');  // output: 3
   putchar('\n');
}

void PutToSea(void)
{
   putchar('2');  // output: 2
   putchar('\n');

   NameOfTheShip();

   putchar('4');  // output: 4
   putchar('\n');
}


void main(void)
{
   putchar('1');  // output: 1
   putchar('\n');
   
   PutToSea();

   putchar('5');  // output: 5
   putchar('\n');
}

