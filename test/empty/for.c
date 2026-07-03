/* for --- test an empty for-loop                           2026-07-01 */

void puts(const char *s);

void main(void)
{
   int rum;

   puts("ship");  // output: ship
   
   for (rum = 4; rum ; rum--)
      ;

   puts("whale"); // output: whale
}

