/* while --- test an empty while-loop                       2026-07-01 */

void puts(const char *s);

void main(void)
{
   int rum;

   puts("tea");   // output: tea
   
   rum = 4;
   while (rum--)
      ;

   puts("sugar"); // output: sugar
}

