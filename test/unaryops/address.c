/* address --- test unary ampersand                         2026-07-04 */

void putu();

int Sugar = 4200;

void main(void)
{
   static int whale;
   int tea;
   int rum;
   
   tea = +42;
   rum = -42;
   
   putu(&whale);  // output: 1389
   
   putu(&tea);    // output: 65530
   
   putu(&rum);    // output: 65528
   
   putu(&Sugar);  // output: 1387
}
