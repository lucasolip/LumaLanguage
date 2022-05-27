#include "Q.h"
BEGIN
STAT(0)
	STR(73725, "%i");
	STR(73721, "\n");
CODE(0)
L 1:
	R6 = R7;
	R7 = R7 - 8;
	R0 = 1;
	I(R6 + -4) = R0;
	R0 = 0;
	I(R6 + -8) = R0;
L 2:
	R0 = I(R6 + -8);
	R1 = I(R6 + 16);
	R0 = R0 < R1;
	IF (!R0) GT(3);
	R0 = I(R6 + -4);
	R1 = I(R6 + 12);
	R0 = R0 * R1;
	I(R6 + -4) = R0;
	R0 = I(R6 + -8);
	R1 = 1;
	R0 = R0 + R1;
	I(R6 + -8) = R0;
	GT(2);
L 3:
	R0 = I(R6 + -4);
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
	R7 = R7 - 0;
	R7 = R7 - 20;
	R0 = 3;
	I(R7 + 12) = R0;
	R0 = 3;
	I(R7 + 16) = R0;
	P(R7+4) = R6;
	P(R7) = 4;
	GT(1);
L 4:	R5 = R7 + 8;
	R7 = R7 + 20;
	R7 = R7 + 0;
	R0 = I(R5);
	I(R7 + 12) = R0;
	R0 = 3;
	I(R7 + 16) = R0;
	P(R7+4) = R6;
	P(R7) = 5;
	GT(1);
L 5:	R5 = R7 + 8;
	R7 = R7 + 20;
	R7 = R7 + 0;
	R0 = I(R5);
	R3 = R0;
	R0 = 6;
	R1 = 73725;
	R2 = R3;
	GT(putf_);
L 6:
	R0 = 7;
	R1 = 73721;
	GT(putf_);
L 7:
	GT(-2);
END