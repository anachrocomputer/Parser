/* Some simple function calls which the code so far is capable of parsing */

// Scalars, all global until we get 'auto', 'static' and 'register' working
int Wave;
char *Bow;
char Tea;
int Sugar;
float Rum;
double Whale;

// Assembler-coded functions in the run-time library
void vduchar(void);
void vdustr(void);
int getchar(void);
void hex2ou(void);
void hex4ou(void);
void hex8ou(void);

void PutToSea(void);
void newline(void);

/* PutToSea --- test assembler-coded VDU output functions */

void PutToSea(void)
{
   float heading;
   
   'P';
   vduchar();
   'u';
   vduchar();
   't';
   vduchar();
   "ToSea()\r\n";
   vdustr();
}


void main(void)
{
   'm';
   vduchar();
   'a';
   vduchar();
   'i';
   vduchar();
   'n';
   vduchar();
   '\r';
   vduchar();
   '\n';
   vduchar();

   PutToSea();
   
   for (Sugar = 42; Sugar; Sugar--) {
      '*';
      vduchar();
   }

   '\r';
   vduchar();
   '\n';
   vduchar();
   
   for (Sugar = 4; Sugar; Sugar--) {
      Wave = getchar();
      
      switch (Wave) {
      case 'A':
      case 'a':
         Bow = "Anchor";
         break;
      case 'B':
      case 'b':
         Bow = "Bowsprit";
         break;
      case 'C':
      case 'c':
         Bow = "Capstan";
         break;
      case 'D':
      case 'd':
         Bow = "Deck";
         break;
      default:
         Bow = "Zenith";
         break;
      }
      
      Bow;
      vdustr();
      '\r';
       vduchar();
      '\n';
      vduchar();
   }
}

