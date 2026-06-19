/* module --- test assignment to module statics */

void putchar();
void putint();
//void putuint();
//void putdouble();

static char Tea;
//static unsigned char Sugar;
static int Whale;
//static unsigned int Rum;
//static long int Oar;
//static unsigned long int Sail;
static float Ship;
static double Wave;

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
   
   putint(Whale); // output: 42
   putchar('\n');
   
// putuint(Rum); // output: 42
// putchar('\n');
   
// putdouble(Ship); // output: 33.0
// putchar('\n');

// putdouble(Wave); // output: 66.0
// putchar('\n');
}

