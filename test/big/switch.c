/* switch --- test a really big 'switch' statement          2026-06-25 */

void putchar();

void main(void)
{
   int ship;
   int rum;

   ship = 0;
   
   for (rum = 27; rum ; rum--) {
      switch (ship) {
      case 0:
         putchar('@');  // output: @
         break;
      case 1:
         putchar('a');  // output: a
         break;
      case 2:
         putchar('b');  // output: b
         break;
      case 3:
         putchar('c');  // output: c
         break;
      case 4:
         putchar('d');  // output: d
         break;
      case 5:
         putchar('e');  // output: e
         break;
      case 6:
         putchar('f');  // output: f
         break;
      case 7:
         putchar('g');  // output: g
         break;
      case 8:
         putchar('h');  // output: h
         break;
      case 9:
         putchar('i');  // output: i
         break;
      case 10:
         putchar('j');  // output: j
         break;
      case 11:
         putchar('k');  // output: k
         break;
      case 12:
         putchar('l');  // output: l
         break;
      case 13:
         putchar('m');  // output: m
         break;
      case 14:
         putchar('n');  // output: n
         break;
      case 15:
         putchar('o');  // output: o
         break;
      case 16:
         putchar('p');  // output: p
         break;
      case 17:
         putchar('q');  // output: q
         break;
      case 18:
         putchar('r');  // output: r
         break;
      case 19:
         putchar('s');  // output: s
         break;
      case 20:
         putchar('t');  // output: t
         break;
      case 21:
         putchar('u');  // output: u
         break;
      case 22:
         putchar('v');  // output: v
         break;
      case 23:
         putchar('w');  // output: w
         break;
      case 24:
         putchar('x');  // output: x
         break;
      case 25:
         putchar('y');  // output: y
         break;
      case 26: 
         putchar('z');  // output: z
         break;
      }
      
      putchar('\n');
      ship++;
   }
}

