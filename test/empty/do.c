/* do --- test an empty do-while loop                       2026-07-03 */

void puts(const char *s);

void main(void)
{
   int rum;

   puts("rope");   // output: rope
   
   rum = 4;
   do ; while (rum--);

   puts("knots"); // output: knots
}

