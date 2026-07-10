/* globalarrays --- test initialisation of global arrays    2026-07-08 */

char NameOfTheWhale[128];
int Mast[2 * 2];
float Cask[14 + 2];
double Funnel[3];
char *CrewNames['*'];
void *ParrotNames['\x0f'];

void puti(const int i);
void putu(const int u);
void putchar(const char c);

void main(void)
{
   // Invalid assignments but we have no array indexing yet
   NameOfTheWhale = 'W';
   Mast = 42;
   
   putchar(NameOfTheWhale);    // output: W
   putchar('\n');   
// putu(sizeof (NameOfTheWhale));  // output: 128
   
   puti(Mast);    // output: 42
// putu(sizeof (Mast));  // output: 8
}

