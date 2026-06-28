/* float --- test float literals                            2026-06-28 */

void putd();

void main(void)
{
   putd(0.0f);  // output: 0.0

   putd(1.0f);  // output: 1.0

   putd(0.f);  // output: 0.0

   putd(1.f);  // output: 1.0

   putd(0.0e3f);  // output: 0.0

   putd(1.0e3f);  // output: 1000.0

   putd(0e3f);  // output: 0.0

   putd(1e3f);  // output: 1000.0
}
