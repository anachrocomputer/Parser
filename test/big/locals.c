/* locals --- test a void function with lots of locals      2026-07-07 */

void putchar(const int c);
void puti(const int i);
void puts(const char *s);

void Wellerman(int sugar);

void Wellerman(const int sugar)
{
   int ship;
   int whale;
   int captain;
   int tea;
   int rum;
   int merchandise;
   int tonguing;
   int wave;
   int sea;         // Stack frame offset > 16
   int harbour;
   double blubber;
   double wellerman;
   double sailor;
   double cabinBoy;
   double bosun;
   double hold;
   double cargo;
   double hatch;
   double keel;
   double d1;    // Just make the stack frame bigger than 128 bytes
   double d2;
   double d3;
   double d4;
   double d5;
   int harpoon;    // Stack frame offset > 128
   
   ship = 'B';
   whale = 42;
   captain = 'P';  // Pugwash
   tea = 'T';
   rum = 'R';  // Arrr
   merchandise = 7;
   tonguing = 1;  // Done
   wave = 32767;
   sea = sugar;    // Copied into local
   harbour = 'H';
   harpoon = sea;  // Copy again further up the stack frame
   // Can't assign to 'double' types
   
   putchar(harpoon);   // Force the assembler to generate a four-byte opcode
   putchar('\n');
}


void main(void)
{
   puts("Put to sea");  // output: Put to sea

   Wellerman('b'); // output: b

   puts("Whale in tow");  // output: Whale in tow
}
