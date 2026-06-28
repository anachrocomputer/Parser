/* comments --- verify that comments are ignored            2026-06-27 */

void putchar();
/* $@` are not used in C */
// $@` are not used in C

/* if (rum) ship(); */
// if (tea) whale();
/* break; continue; */
// continue; break;

void main(void)
{
   // return;
   /* return; */
   putchar('a');  // output: a
   putchar('\n');
}
/* Source file ends with a comment */
