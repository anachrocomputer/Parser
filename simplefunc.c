/* Some simple function calls which the code so far is capable of parsing */

// Scalars
int Wave;
char *Bow;
char Tea;
int Sugar;
float Rum;
double Whale;

void PutToSea(void);

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

