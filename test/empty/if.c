/* if --- test an empty if statement                        2026-07-01 */

void puts(const char *s);

void main(void)
{
   int rum;

   puts("Wellerman");   // output: Wellerman
   
   rum = 4;
   
   if (rum)
      ;

   puts("Billy O'Tea"); // output: Billy O'Tea
}

