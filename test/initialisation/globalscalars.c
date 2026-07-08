/* globalscalars --- test initialisation of global scalars  2026-07-08 */

char Ship = 'S' + 1;
int Sailors = 7 * 6;
float Soon = 12.0f;
double Cargo = 153e4;
float Whale = 12.3e4f;
double Rum = 0.3e-2;
int Sugar = 0xe+2;    // Ill-formed
int Tea = (0xE) + 2;

// Pointers
void *Sail = 0;
char **Billy = 0;
int *Heave = 0;
float **Rope = 0;
double *Anchor = 0;

void puti(const int i);
void putu(const int u);
void putchar(const char c);

void main(void)
{
   putchar(Ship);    // output: T
   putchar('\n');
   
   puti(Sailors);    // output: 42
   
   puti(Sugar);      // output: 16
   
   puti(Tea);        // output: 16
   
   putu(Sail);       // output: 0
   
   putu(Billy);      // output: 0
   
   putu(Heave);      // output: 0
   
   putu(Rope);       // output: 0
   
   putu(Sail);       // output: 0
   
   putu(Anchor);     // output: 0
}

