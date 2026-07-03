/* register --- test assignment to register variables       2026-06-21 */

void putchar(const char c);
void puti(const int i);

int main(void)
{
   register int whale;

   whale = 42;
   
   putchar(whale);  // output: *
   putchar('\n');
      
   puti(whale);     // output: 42

   whale = 'A';
   
   putchar(whale);  // output: A
   putchar('\n');
}

