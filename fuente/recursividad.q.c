#include "Q.h"
BEGIN
STAT(0)
	DAT(73724, I, 0);
	STR(73721, "%i");
	STR(73717, "\n");
CODE(0)
L 1:
	R6 = R7;
	R7 = R7 - 0;
	R0 = I(R6 + 12);
	R1 = 0;
	R0 = R0 == R1;
	R1 = I(R6 + 12);
	R2 = 1;
	R1 = R1 == R2;
	R0 = R0 | R1;
	IF (!R0) GT(2);
	R0 = 1;
	I(R6 + 8) = R0;
	R7 = R6;
	R6 = P(R7 + 4);
	R5 = P(R7);
	GT(R5);
L 2:
	R0 = I(R6 + 12);
	R7 = R7 - 4;
	P(R7 + 0) = R0;
	R7 = R7 - 16;
	R0 = I(R6 + 12);
	R1 = 1;
	R0 = R0 - R1;
	I(R7 + 12) = R0;
	P(R7+4) = R6;
	P(R7) = 3;
	GT(1);
L 3:	R5 = R7 + 8;
	R7 = R7 + 16;
	R0 = P(R7 + 0);
	R7 = R7 + 4;
	R1 = I(R5);
	R0 = R0 * R1;
	I(R6 + 8) = R0;
	R7 = R6;
	R6 = P(R7 + 4);
	R5 = P(R7);
	GT(R5);
	R7 = R6;
	R6 = P(R7 + 4);
	R5 = P(R7);
L 4:
	R6 = R7;
	R7 = R7 - 0;
	R0 = I(R6 + 12);
	R1 = 0;
	R0 = R0 == R1;
	R1 = I(R6 + 12);
	R2 = 1;
	R1 = R1 == R2;
	R0 = R0 | R1;
	IF (!R0) GT(5);
	R0 = 1;
	I(R6 + 8) = R0;
	R7 = R6;
	R6 = P(R7 + 4);
	R5 = P(R7);
	GT(R5);
L 5:
	R7 = R7 - 0;
	R7 = R7 - 16;
	R0 = I(R6 + 12);
	R1 = 1;
	R0 = R0 - R1;
	I(R7 + 12) = R0;
	P(R7+4) = R6;
	P(R7) = 6;
	GT(4);
L 6:	R5 = R7 + 8;
	R7 = R7 + 16;
	R7 = R7 + 0;
	R0 = I(R5);
	R7 = R7 - 4;
	P(R7 + 0) = R0;
	R7 = R7 - 16;
	R0 = I(R6 + 12);
	R1 = 2;
	R0 = R0 - R1;
	I(R7 + 12) = R0;
	P(R7+4) = R6;
	P(R7) = 7;
	GT(4);
L 7:	R5 = R7 + 8;
	R7 = R7 + 16;
	R0 = P(R7 + 0);
	R7 = R7 + 4;
	R1 = I(R5);
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
	GT(R5);
L 0:
	R0 = 0;
	I(73724) = R0;
L 8:
	R0 = I(73724);
	R1 = 10;
	R0 = R0 < R1;
	IF (!R0) GT(9);
	R7 = R7 - 0;
	R7 = R7 - 16;
	R0 = I(73724);
	I(R7 + 12) = R0;
	P(R7+4) = R6;
	P(R7) = 10;
	GT(4);
L 10:	R5 = R7 + 8;
	R7 = R7 + 16;
	R7 = R7 + 0;
	R0 = I(R5);
	R7 = R7 - 4;
	P(R7 + 0) = R0;
	R7 = R7 - 16;
	R0 = I(73724);
	I(R7 + 12) = R0;
	P(R7+4) = R6;
	P(R7) = 11;
	GT(1);
L 11:	R5 = R7 + 8;
	R7 = R7 + 16;
	R0 = P(R7 + 0);
	R7 = R7 + 4;
	R1 = I(R5);
	R0 = R0 + R1;
	R3 = R0;
	R0 = 12;
	R1 = 73721;
	R2 = R3;
	GT(putf_);
L 12:
	R0 = 13;
	R1 = 73717;
	GT(putf_);
L 13:
	R0 = I(73724);
	R1 = 1;
	R0 = R0 + R1;
	I(73724) = R0;
	GT(8);
L 9:
	GT(-2);
END