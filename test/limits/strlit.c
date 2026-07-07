/* strlit --- maximal string literal                        2026-07-07 */

void puts(const char *s);

void main(void)
{
   // ANSI/ISO C89 requires the compiler to accept at least 509 characters in a string literal
   // C99 expands this to a minimum of 4095
   char *shanty;
   
   shanty = "There Once Was A Ship That Put To Sea And The Name Of The Ship Was The Billy O' Tea The Winds Blew Up Her Bow Dipped Down Blow My Bully Boys Blow She Had Not Been Two Weeks From Shore When Down On Her A Right Whale Bore The Captain Called All Hands And Swore He'd Take That Whale In Tow Soon May The Wellerman Come To Bring Us Sugar And Tea And Rum One Day When The Tonguing Is Done We'll Take Our Leave And Go Soon May The Wellerman Come To Bring Us Sugar And Tea And Rum One Day When The Tonguing Is Done We"; // 509 chars
   
   puts(shanty);  // output: There Once Was A Ship That Put To Sea And The Name Of The Ship Was The Billy O' Tea The Winds Blew Up Her Bow Dipped Down Blow My Bully Boys Blow She Had Not Been Two Weeks From Shore When Down On Her A Right Whale Bore The Captain Called All Hands And Swore He'd Take That Whale In Tow Soon May The Wellerman Come To Bring Us Sugar And Tea And Rum One Day When The Tonguing Is Done We'll Take Our Leave And Go Soon May The Wellerman Come To Bring Us Sugar And Tea And Rum One Day When The Tonguing Is Done We
}


