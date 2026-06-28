/* double --- test double literals                          2026-06-28 */

void putd();

void main(void)
{
   putd(0.0);  // output: 0.0

   putd(1.0);  // output: 1.0

   putd(0.);  // output: 0.0

   putd(1.);  // output: 1.0

   putd(0.0e3);  // output: 0.0

   putd(1.0e3);  // output: 1000.0

   putd(0e3);  // output: 0.0

   putd(1e3);  // output: 1000.0
}
