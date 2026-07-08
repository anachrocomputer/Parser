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
   //NameOfTheWhale = 'W';
   //Mast = 42;
   
   //putchar(NameOfTheWhale);    // output: W
   putchar('\n');
   
   //puti(Mast);    // output: 42
}

