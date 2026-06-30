/* fib --- calculate a Fibonacci number recursively         2026-06-24 */

void puti();

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
      puti(fib(i));  // output: 0
                     // output: 1
                     // output: 1
                     // output: 2
                     // output: 3
                     // output: 5
                     // output: 8
                     // output: 13
                     // output: 21
                     // output: 34
                     // output: 55
                     // output: 89
                     // output: 144
                     // output: 233
                     // output: 377
                     // output: 610
                     // output: 987
                     // output: 1597
                     // output: 2584
                     // output: 4181
                     // output: 6765
                     // output: 10946
                     // output: 17711
                     // output: 28657

}

