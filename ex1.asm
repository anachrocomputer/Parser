 setdp $0
 org $0400
appEntry
_main
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
 
 leax -2,u  ; &a
 leax -4,u  ; &b
 leax -6,u  ; &c
 leax -15,u
 leax -16,u
 leax -17,u
 leax -18,u
 
 ldd  -16,u     ; Load auto int
 ldd  -17,u     ; Load auto int

 ldq  -16,u     ; Load auto long
 ldq  -20,u     ; Load auto long
 
 leax l0001,PCR ; &bytes
 leay i0002,PCR ; &state
 leau c0003,PCR ; &letter
 
 ldd  ,x        ; Load int via pointer
 ldq  ,x        ; Load long int via pointer
 
 std  -16,u     ; Store auto int
 std  -18,u     ; Store auto int
 
 stq  -16,u     ; Store auto long
 stq  -20,u     ; Store auto long

 ldx  #l0001 ; &bytes
 ldy  #i0002 ; &state
 ldu  #c0003 ; &letter
 
 jmp  r0001     ; return
 jmp  r0001,PCR ; return
 bra  r0001     ; return
 lbra r0001     ; return
 
 ldq  l0001 ; bytes
 ldq  l0001,PCR ; bytes
 
 ldq  #65536    ; 32-bit constant
 stq  l0001     ; Store 'bytes'
 stq  l0001,PCR ; Store 'bytes'

 ldd  i0002 ; state
 ldd  i0002,PCR ; state
 
 ldb  c0003 ; letter
 sex
 ldb  c0003,PCR ; letter
 sex
 
 ldd  #42
 std  i0002 ; state
 
 ldd  #66
 stb  c0003 ; letter
 
 ldd #42    ; return 42;
 
r0001 
 tfr u,s
 puls u
 rts
l0001 fqb  65536 ; static long int bytes = 256L * 256L;
i0002 fdb  17  ; static int state = 17;
c0003 fcb  65  ; static char letter = 'A';
 end appEntry
