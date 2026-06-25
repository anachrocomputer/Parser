/* fib --- calculate a Fibonacci number recursively         2026-06-24 */

#include <stdio.h>

int fib(const int n)
{
   if (n < 2)
      return (n);
   else
      return (fib(n - 2) + fib(n - 1));
}

int main(void)
{
   for (int i = 0; i < 47; i++)
      printf("%d: %d\n", i, fib(i));
}

