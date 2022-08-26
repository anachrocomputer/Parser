/* Some simple control structures which the code so far is capable of parsing */

// Scalars
char Tea;
int Sugar;
float Rum;
double Whale;

char *NameOfTheShip(void)
{
   float heading;
   double knots;
   
   {
      33.3; // 'double' constant will generate a NOP in the assembly language
      return 999;
   }
   42;   // The only <expression> we know so far is an integer constant or an identifier
   for;
   '*';
   if;
   0xff;
   do {
      123L;
      break;
      5678;
      continue;
      do {
         0x7FFF;
         break;
         1000;
         continue;
         999;
         return Sugar;
      } while (0);
      0377;
   } while (Rum);
   switch;
   0;
   return Rum;
   do Tea; while (1);
   goto sea;
   Tea;
   while (Rum) Tea;
   Sugar;
   while (Tea) {
      3456;
      break;
      6789;
      continue;
      0x9abc;
      return 0377;
   }
   Rum;
   goto harbour;
}

