/* static --- test assignment to local statics */

void putchar();
void putint();
//void putuint();
//void putdouble();

int main(void)
{
   static char tea;
   //static unsigned char sugar;
   static int whale;
   //static unsigned int rum;
   //static long int oar; 
   //static unsigned long int sail;
   static float ship;
   static double wave;

   tea = 't';
// sugar = 's';
   whale = 42;
// rum = 42;
// oar = 420000L;
// sail = 840000UL;
// ship = 33.0;
// wave = 66.0;
   
   putchar(tea);  // output: t
   putchar('\n');
   
// putchar(sugar);  // output: s
// putchar('\n');
   
// putint(whale); // output: 42
// putchar('\n');
   
// putuint(rum); // output: 42
// putchar('\n');
   
// putdouble(ship); // output: 33.0
// putchar('\n');

// putdouble(wave);  // output: 66.0
// putchar('\n');
}

