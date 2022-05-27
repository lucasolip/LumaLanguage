#include "Q.h"
BEGIN
STAT(0)
	STR(73703, "Las sumas son iguales\n");
CODE(0)
L 1:
	R6 = R7;
	R7 = R7 - 0;
	R0 = I(R6 + 12);
	R1 = I(R6 + 16);
	R0 = R0 + R1;
	I(R6 + 8) = R0;
	R7 = R6;
	R6 = P(R7 + 4);
	R5 = P(R7);
	GT(R5);
	R7 = R6;
	R6 = P(R7 + 4);
	R5 = P(R7);
	GT(R5);
L 0:
	R7 = R7 - 0;
	R7 = R7 - 20;
	R0 = 2;
	I(R7 + 12) = R0;
	R0 = 3;
	I(R7 + 16) = R0;
	P(R7+4) = R6;
	P(R7) = 2;
	GT(1);
L 2:	R5 = R7 + 8;
	R7 = R7 + 20;
	R7 = R7 + 0;
	R0 = I(R5);
	R1 = 2;
	R2 = 3;
	R1 = R1 + R2;
	R0 = R0 == R1;
	IF (!R0) GT(3);
	R0 = 4;
	R1 = 73703;
	GT(putf_);
L 4:
L 3:
	GT(-2);
END