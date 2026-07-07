/* identifier --- test a long identifier name               2026-07-07 */

void putchar(const char c);
void puti(const int i);

int main(void)
{
   char tea;
   int whale;
   int thereOnceWasAShipThatPutToSea;
   int andTheNameOfTheShipWasTheBillyOTea;
   int soonMayTheWellermanComeToBringUsSugarAndTeaAndRum;

   tea = 't';
   whale = 42;
   thereOnceWasAShipThatPutToSea = 4200;
   andTheNameOfTheShipWasTheBillyOTea = 'B';
   soonMayTheWellermanComeToBringUsSugarAndTeaAndRum = 32767;
   
   putchar(tea);  // output: t
   putchar('\n');
   
   puti(whale);   // output: 42
   
   puti(thereOnceWasAShipThatPutToSea); // output: 4200
   puti(andTheNameOfTheShipWasTheBillyOTea); // output: 66
   puti(soonMayTheWellermanComeToBringUsSugarAndTeaAndRum); // output: 32767
}

