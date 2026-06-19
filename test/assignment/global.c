/* global --- test assignment to globals */

void putchar();
void putint();
//void putuint();
//void putdouble();

char Tea;
//unsigned char Sugar;
int Whale;
//unsigned int Rum;
//long int Oar;
//unsigned long int Sail;
float Ship;
double Wave;

int main(void)
{
   Tea = 't';
// Sugar = 's';
   Whale = 42;
// Rum = 42;
// Oar = 420000L;
// Sail = 840000UL;
// Ship = 33.0;
// Wave = 66.0;
   
   putchar(Tea);  // output: t
   putchar('\n');
   
// putchar(Sugar);  // output: s
// putchar('\n');
   
// putint(Whale); // output: 42
// putchar('\n');
   
// putuint(Rum); // output: 42
// putchar('\n');
   
// putdouble(Ship); // output: 33.0
// putchar('\n');

// putdouble(Rum);  // output: 66.0
// putchar('\n');
}

