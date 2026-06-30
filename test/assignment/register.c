/* register --- test assignment to register variables       2026-06-21 */

void putchar();
void putint();

int main(void)
{
   register int whale;

   whale = 42;
   
   putchar(whale);  // output: *
   putchar('\n');
      
// putint(whale); // output: 42
// putchar('\n');

   whale = 'A';
   
   putchar(whale);  // output: A
   putchar('\n');
}

