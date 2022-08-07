
main
 pshs u
 tfr s,u
 leas -6,s
 
 ldd #1     ; int a = 1;
 std -2,u
 
 ldd #2     ; int b = 2;
 std -4,u
 
 ldd -2,u   ; c = a + b;
 addd -4,u
 std -6,u
 
 ldd #42    ; return 42;
 
 tfr u,s
 puls u
 rts
 
