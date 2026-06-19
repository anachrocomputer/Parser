/* local --- test assignment to locals */

void putchar();
void putint();
//void putuint();
//void putdouble();

int main(void)
{
   char tea;
   //unsigned char sugar;
   int whale;
   //unsigned int rum;
   //long int oar;
   //unsigned long int sail;
   float ship;
   double wave;

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

