/* ifelse --- test an empty if-else statement               2026-07-01 */

void puts(const char *s);

void main(void)
{
   int rum;

   puts("sugar"); // output: sugar
   
   rum = 4;
   
   if (rum)
      ;
   else
      ;

   puts("tea");   // output: tea
}

