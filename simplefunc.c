/* Some simple function calls which the code so far is capable of parsing */

// Scalars, all global until we get 'auto', 'static' and 'register' working
int Wave;
char *Bow;
int Tea;
int Sugar;
int Rum;
int Whale;

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


/* SugarAndTeaAndRum --- test nested 'for' loops */

void SugarAndTeaAndRum(void)
{
   for (Rum = 10; Rum; Rum--) {
      Sugar = 'A';
      
      for (Tea = 40; Tea; Tea--) {
         Sugar++;
         vduchar();
      }
      
      newline();
   }
}


/* DippedDown --- inner 'for' loop with another 'register' loop counter */

void DippedDown(void)
{
   register int tea;
   static int sugar;
   
   sugar = 'A';
   
   for (tea = 40; tea; tea--) {
      sugar++;
      vduchar();
   }
}


/* HerBowDippedDown --- test nested 'for' loops with 'register' loop counter */

void HerBowDippedDown(void)
{
   register int rum;
   
   for (rum = 10; rum; rum--) {
      DippedDown();
      newline();
   }
}


/* HeaveHo --- test 'if' statements */

void HeaveHo(void)
{
   Sugar = 0;
   
   for (Rum = 10; Rum; Rum--) {
      if (Sugar) {
         "Even ";
         vdustr();
         Sugar--;
      }
      else {
         "Odd ";
         vdustr();
         Sugar++;
      }
   }
   
   newline();
}


/* TakeOurLeaveAndGo --- test 'while' loops */

void TakeOurLeaveAndGo(void)
{
   Rum = 10;
   while (Rum) {
      Tea = 40;
      Sugar = 'A';
      while (Tea) {
         Tea--;
         Sugar++;
         vduchar();
      }
      Rum--;
      newline();
   }
}


/* SixteenMen --- test hex output functions */

void SixteenMen(void)
{
   register int hex;
   
   hex = 0xAA;
   
   for (Tea = 15; Tea; Tea--) {
      hex++;
      hex2ou();
      ' ';
      vduchar();
   }

   newline();
   
   hex = 0xbeef;
   
   hex;
   hex4ou();
   newline();
   
   hex = 0xdead;

   hex;
   hex8ou();
   newline();
}


void newline(void)
{
   '\r';
   vduchar();
   '\n';
   vduchar();
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
   newline();

   PutToSea();
   
   for (Sugar = 42; Sugar; Sugar--) {
      '*';
      vduchar();
   }

   newline();

   Whale = 1;
   
   while (Whale) {
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
      case 'F':
      case 'f':
         SugarAndTeaAndRum();
         Bow = "Fo'c'sle";
         break;
      case 'H':
      case 'h':
         SixteenMen();
         Bow = "Heave-Ho";
         break;
      case 'I':
      case 'i':
         HeaveHo();
         Bow = "Iceberg";
         break;
      case 'Q':
      case 'q':
         Whale = 0;
         Bow = "Queequeg";
         break;
      case 'R':
      case 'r':
         HerBowDippedDown();
         Bow = "Arrrr";
         break;
      case 'W':
      case 'w':
         TakeOurLeaveAndGo();
         Bow = "Whale";
         break;
      default:
         Bow = "Zenith";
         break;
      }
      
      Bow;
      vdustr();
      newline();
   }
}

