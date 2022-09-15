/* Some simple declarations which the code so far is capable of parsing */

// Scalars
char Tea;
int Sugar;
float Rum;
double Whale;

// Initialised scalars
char Ship = 'S' + 1;
int Sailors = 7 * 6;
float Soon = 12.0f;
double Cargo = 153e4;

// Pointers
void *Sail = 0;
char **Billy = 0;
int *Heave = 0;
float **Rope = 0;
double *Anchor = 0;

// Arrays
char NameOfTheWhale[128];
int Mast[2 * 2];
float Cask[14 + 2];
double Funnel[3];
char *CrewNames['*'];
void *ParrotNames['\x0f'];

// Functions
void PutToSea();
char SeenWhale();
int Blow();
float Buoyancy(void);
double ConsumeRum(void);

char *NameOfTheShip();

char *NameOfTheShip(void)
{
   int tea;
   static char *namePtr;
   char *billy;
   char captainsInitial;
   static char bosunsInitial;
   register float heading;
   static float buoy;
   auto double knots;
   static int seaState;
   static double funnel;
   
   namePtr;
   bosunsInitial;
   buoy;
   seaState++;
   funnel;
   
   tea++;
   billy;
   captainsInitial--;
   knots;
   
   heading;

   return 42;
}

int TonguingIsDone = 0;  // Cutting strips of blubber

void TakeOurLeaveAndGo(void)
{
   return;
}

void main(void)
{
   Sail = NameOfTheShip();
}
