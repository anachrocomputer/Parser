/* string --- test string literals                          2026-06-28 */

void puts();

void main(void)
{
   puts("");  // output: 
   
   puts("a");  // output: a
   
   puts("abcdefghijklmnopqrstuvwxyz");  // output: abcdefghijklmnopqrstuvwxyz
   
   puts("!\"#$%^&*()_`@~");  // output: !"#$%^&*()_`@~
   // TODO \n \t \a \x0d \xff \012 etc. But how to write the expected output?
}
