#include "Q.h"
# 1 "<stdin>"
# 1 "<built-in>"
# 1 "<command-line>"
# 31 "<command-line>"
# 1 "/usr/include/stdc-predef.h" 1 3 4
# 32 "<command-line>" 2
# 1 "<stdin>"
# 1 "Qlib.h" 1
# 2 "<stdin>" 2
BEGIN
STAT(0)
 STR(73721, "Sí\n");
 STR(73715, "No\n");
CODE(0)
L 1:
 R6 = R7;
 R7 = R7 - 0;
 R0 = U(R6 + 12);
 IF (!R0) GT(2);
 R0 = 3;
 R1 = 73721;
 GT(-12);
L 3:
 GT(4);
L 2:
 R0 = 5;
 R1 = 73715;
 GT(-12);
L 5:
L 4:
 R7 = R6;
 R6 = P(R7 + 4);
 R5 = P(R7);
 GT(R5);
L 0:
 R7 = R7 - 0;
 R7 = R7 - 16;
 R0 = -1;
 R1 = -1;
 R0 = R0 & R1;
 U(R7 + 12) = R0;
 P(R7+4) = R6;
 P(R7) = 6;
 GT(1);
L 6: R5 = R7 + 8;
 R7 = R7 + 16;
 R7 = R7 + 0;
 R0 = P(R5);
 GT(-2);
END
