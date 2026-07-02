/* arity_var --- test a void function with variable arity   2026-07-02 */

void puts(const char *s);

void main(void)
{
   // Functions must be callable with more arguments than they are
   // declared to take. This is so that functions like 'printf()'
   // can be written, and 'varargs' will work.
   int tea;
   char *rum;
   
   rum = "rum";
   
   puts("sugar");               // output: sugar
   puts("sugar", "tea");        // output: sugar
   puts("sugar", "tea", "rum"); // output: sugar
   puts("sugar", tea, rum);     // output: sugar
}

