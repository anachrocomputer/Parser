/* fib --- calculate a Fibonacci number recursively         2026-06-24 */

void puti();
int fib();

int fib(const int n)
{
   if (n < 2)
      return (n);
   else
      return (fib(n - 2) + fib(n - 1));
}

int main(void)
{
   int i;
   
   for (i = 0; i < 24; i++)
      puti(fib(i));
}

