/* function --- test an empty function called from main */

void putchar();

void PutToSea(void)
{
}


void main(void)
{
   putchar('a');  // output: a
   putchar('\n');
   
   PutToSea();

   putchar('b');  // output: b
   putchar('\n');
}

