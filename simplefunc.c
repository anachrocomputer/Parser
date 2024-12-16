/* Some simple function calls which the code so far is capable of parsing */

// Scalars, all global until we get 'auto', 'static' and 'register' working
int Wave;
char *Bow;
int Tea;
int Sugar;
int Rum;
int Whale;

// Assembler-coded functions in the run-time library
void vduchar(const char ch);
void vdustr(const char *str);
int getchar(void);
void hex2ou(int h);
void hex4ou(int h);
void hex8ou(int h);
void exit(void);

void PutToSea(const int heading, char ocean);
void newline(void);

/* PutToSea --- test assembler-coded VDU output functions */

void PutToSea(const int heading, char ocean)
{
   int rum;
   const int parrot;
   
   rum = 5;
   
   vduchar('P');
   vduchar('u');
   vduchar('t');
   vdustr("ToSea()\r\n");
   
   parrot;
   heading;
   ocean;
}


/* SugarAndTeaAndRum --- test nested 'for' loops using externs */

void SugarAndTeaAndRum(void)
{
   for (Rum = 10; Rum; Rum--) {
      Sugar = 'A';
      
      for (Tea = 40; Tea; Tea--) {
         vduchar(Sugar++);
      }
      
      newline();
   }
}


/* ToBringUs --- test nested 'for' loops using 'auto' loop counters */

void ToBringUs(void)
{
   auto int rum;
   auto int tea;
   auto int sugar;
   
   for (rum = 10; rum; rum--) {
      sugar = 'A';
      
      for (tea = 40; tea; tea--) {
         vduchar(sugar++);
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
      vduchar(sugar++);
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
         vdustr("Even ");
         Sugar--;
      }
      else {
         vdustr("Odd ");
         Sugar++;
      }
   }
   
   newline();
}


/* TakeOurLeaveAndGo --- test 'exit()' function */

void TakeOurLeaveAndGo(void)
{
   vdustr("The tonguing is done");
   newline();
   
   exit();
   
   vdustr("We're gone");  // Should not print
}


/* WhaleInTow --- test 'while' loops */

char *WhaleInTow(void)
{
   auto int rum;
   
   rum = 10;
   while (rum) {
      Tea = 40;
      Sugar = 'A';
      while (Tea) {
         Tea--;
         vduchar(Sugar++);
      }
      rum--;
      newline();
   }
   
   return ("Whale");
}


/* SixteenMen --- test hex output functions */

void SixteenMen(void)
{
   register int hex;
   
   hex = 0xAA;
   
   for (Tea = 15; Tea; Tea--) {
      hex2ou(hex++);
      vduchar(' ');
   }

   newline();
   
   hex = 0xbeef;
   
   hex4ou(hex);
   newline();
   
   hex = 0xdead;

   hex8ou(hex);
   newline();
}


/* Cargo --- test static variable acting as a state machine */

char *Cargo(void)
{
   static int cargo; // Implicitly initialised to 0
   char *merchandise;
   
   switch (cargo) {
   case 0:
      merchandise = "Sugar";
      cargo++;
      break;
   case 1:
      merchandise = "Tea";
      cargo++;
      break;
   case 2:
      merchandise = "Rum";
      cargo = 0;
   }
   
   return (merchandise);
}


/* ThereOnceWasAShip --- test static storage class */

void ThereOnceWasAShip(void)
{
   int hold;
   char *goods;
   
   for (hold = 5; hold; hold--) {
      goods = Cargo();
      
      hex2ou(hold);
      vduchar(':');
      vdustr(goods);
      vduchar(' ');
   }
   
   newline();
}


void newline(void)
{
   vduchar('\r');
   vduchar('\n');
}


void main(void)
{
   vduchar('m');
   vduchar('a');
   vduchar('i');
   vduchar('n');
   newline();

   PutToSea(270, 'A');
   
   for (Sugar = 42; Sugar; Sugar--) {
      vduchar('*');
   }

   newline();

   Whale = 1;
   
   while (Whale) {
      Wave = getchar();
      
      switch (Wave) {
      case 'A':
      case 'a':
         ToBringUs();
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
      case 'S':
      case 's':
         ThereOnceWasAShip();
         Bow = "Sea";
         break;
      case 'W':
      case 'w':
         Bow = WhaleInTow();
         break;
      case 'X':
      case 'x':
         TakeOurLeaveAndGo();
         Bow = "XXX";
         break;
      default:
         Bow = "Zenith";
         break;
      }
      
      vdustr(Bow);
      newline();
   }
}

