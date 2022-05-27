#include "Q.h"
BEGIN
STAT(0)
	STR(73687, "Introduzca un año después de Cristo: ");
	DAT(73683, I, 0);
	STR(73666, "Año bisiesto\n");
	STR(73648, "Año ordinario\n");
CODE(0)
L 1:
	R6 = R7;
	R7 = R7 - 0;
	R0 = I(R6 + 12);
	R1 = 4;
	R0 = R0 % R1;
	R1 = 0;
	R0 = R0 == R1;
	R1 = I(R6 + 12);
	R2 = 100;
	R1 = R1 % R2;
	R2 = 0;
	R1 = R1 != R2;
	R0 = R0 & R1;
	R1 = I(R6 + 12);
	R2 = 400;
	R1 = R1 % R2;
	R2 = 0;
	R1 = R1 == R2;
	R0 = R0 | R1;
	U(R6 + 8) = R0;
	R7 = R6;
	R6 = P(R7 + 4);
	R5 = P(R7);
	GT(R5);
	R7 = R6;
	R6 = P(R7 + 4);
	R5 = P(R7);
	GT(R5);
L 0:
	R0 = 2;
	R1 = 73687;
	GT(putf_);
L 2:
	R0 = 3;
	GT(getInteger_);
L 3:
	R0 = R1;
	I(73683) = R0;
L 4:
	R0 = I(73683);
	R1 = 1;
	R1 = -R1;
	R0 = R0 != R1;
	IF (!R0) GT(5);
	R7 = R7 - 0;
	R7 = R7 - 16;
	R0 = I(73683);
	I(R7 + 12) = R0;
	P(R7+4) = R6;
	P(R7) = 6;
	GT(1);
L 6:	R5 = R7 + 8;
	R7 = R7 + 16;
	R7 = R7 + 0;
	R0 = U(R5);
	IF (!R0) GT(7);
	R0 = 8;
	R1 = 73666;
	GT(putf_);
L 8:
	GT(9);
L 7:
	R0 = 10;
	R1 = 73648;
	GT(putf_);
L 10:
L 9:
	R0 = 11;
	GT(getInteger_);
L 11:
	R0 = R1;
	I(73683) = R0;
	GT(4);
L 5:
	GT(-2);
END