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
   for (4; Tea; Tea++)  // for-loops work but cannot have empty expressions yet
      Rum;
   '*';
   for (40; Rum; Rum--) {
      if (Tea)
         break;
      400;
      if (Sugar)
         continue;
      4000;
      if (Whale)
         return (17);
   }
   if (0) return;
   0xff;
   do {
      123L;
      break;
      (5678);  // Expressions may be bracketed
      if (Tea) continue;
      do {
         0x7FFF;
         if (Rum) break;
         1000;
         continue;
         999;
         return (Sugar);
      } while (0);
      0377;
   } while (Rum--); // Variables may be incremented or decremented
   switch (Sugar) {
      456;     // Unreachable code
      break;
   case 42:
      789;     // Fall through
   case (42 * 2) + 1:   // Simple constant integer expressions are allowed
   case 64 * 2:
      return "\"\t\v\f\\\r\n";
   default:
      "Hello, world\n";
   }
   0;
   return Rum;
   do Tea; while (1);
   goto sea;
   Tea++;
   while (Rum) Tea;
   Sugar--;
   while (Tea) {
      '\'';
      break;
      '\t';
      continue;
      '0';
      return 0377;
   }
   Rum;
   if (Tea) {
      '\n';
      return (0xaaaa);
      '\\';
   }
   Whale;
   for (444; Whale; Whale--)
      for (555; Tea; Tea++) {
         if (Sugar)
            break;
         if (Rum)
            continue;
      }
   goto harbour;
}

