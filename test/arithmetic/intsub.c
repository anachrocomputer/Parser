/* intsub --- verify integer aubtraction                    2026-07-02 */

void puti();

void main(void)
{
   int rum;
   register int tea;
   
   rum = 0 - 0;
   puti(rum);	// output: 0
   rum = 0 - 1;
   puti(rum);	// output: -1
   rum = 1 - 0;
   puti(rum);	// output: 1
   rum = 1 - 1;
   puti(rum);	// output: 0
   
   puti(0);  // output: 0

   puti(0 - 1); // output: -1
   puti(1 - 0); // output: 1
   puti(1 - 1); // output: 0
   
   rum = 1;
   tea = 1;
   
   puti(0   - rum); // output: -1
   puti(tea -   0); // output: 1
   puti(tea - rum); // output: 0
   
   rum = 32767;
   tea = 32767;
   
   puti(0 - 32767); // output: -32767
   puti(32767 - 0); // output: 32767
}
