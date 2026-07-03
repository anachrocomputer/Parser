/* function --- test an empty function called from main     2026-06-22 */

void puts(const char *s);

void PutToSea(void)
{
}


void main(void)
{
   puts("harbour");   // output: harbour
   
   PutToSea();

   puts("high seas"); // output: high seas
}

