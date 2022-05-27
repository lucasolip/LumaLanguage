#include "Q.h"
BEGIN
STAT(0)
	DAT(73724, F, 0.0);
	DAT(73720, F, 0.0);
	DAT(73716, F, 0.0);
	DAT(73712, F, 0.0);
	DAT(73708, F, 0.0);
	DAT(73704, F, 0.0);
	STR(73691, "Posición: ");
	STR(73688, "%f");
	STR(73684, ", ");
	STR(73681, "%f");
	STR(73677, "\n");
	STR(73625, "Introduzca el número de pasos de la simulación: ");
	DAT(73621, I, 0);
	STR(73617, "\n");
	DAT(73613, I, 0);
CODE(0)
L 1:
	R6 = R7;
	R7 = R7 - 8;
	RR0 = F(73708);
	RR1 = F(73724);
	RR0 = RR0 - RR1;
	F(R6 + -4) = RR0;
	RR0 = F(73704);
	RR1 = F(73720);
	RR0 = RR0 - RR1;
	F(R6 + -8) = RR0;
	RR0 = F(73716);
	RR1 = F(R6 + -4);
	RR0 = RR0 + RR1;
	F(73716) = RR0;
	RR0 = F(73712);
	RR1 = F(R6 + -8);
	RR0 = RR0 + RR1;
	F(73712) = RR0;
	RR0 = F(73724);
	RR1 = F(73716);
	RR0 = RR0 + RR1;
	F(73724) = RR0;
	RR0 = F(73720);
	RR1 = F(73712);
	RR0 = RR0 + RR1;
	F(73720) = RR0;
	R7 = R6;
	R6 = P(R7 + 4);
	R5 = P(R7);
L 2:
	R6 = R7;
	R7 = R7 - 8;
	R0 = 3;
	R1 = 73691;
	GT(putf_);
L 3:
	RR0 = F(73724);
	RR3 = RR0;
	R0 = 4;
	R1 = 73688;
	RR2 = RR3;
	GT(putfloat_);
L 4:
	R0 = 5;
	R1 = 73684;
	GT(putf_);
L 5:
	RR0 = F(73720);
	RR3 = RR0;
	R0 = 6;
	R1 = 73681;
	RR2 = RR3;
	GT(putfloat_);
L 6:
	R0 = 7;
	R1 = 73677;
	GT(putf_);
L 7:
	R7 = R6;
	R6 = P(R7 + 4);
	R5 = P(R7);
	GT(R5);
	GT(R5);
L 0:
	RR0 = 0.000000;
	F(73724) = RR0;
	RR0 = 0.000000;
	F(73720) = RR0;
	RR0 = 2.100000;
	F(73716) = RR0;
	RR0 = 1.200000;
	R0 = -R0;
	F(73712) = RR0;
	RR0 = 1.000000;
	R0 = -R0;
	F(73708) = RR0;
	RR0 = 1.000000;
	R0 = -R0;
	F(73704) = RR0;
	R0 = 8;
	R1 = 73625;
	GT(putf_);
L 8:
	R0 = 9;
	GT(getInteger_);
L 9:
	R0 = R1;
	I(73621) = R0;
	R0 = 10;
	R1 = 73617;
	GT(putf_);
L 10:
	R0 = 0;
	I(73613) = R0;
L 11:
	R0 = I(73613);
	R1 = I(73621);
	R0 = R0 < R1;
	IF (!R0) GT(12);
	R7 = R7 - 0;
	R7 = R7 - 12;
	P(R7+4) = R6;
	P(R7) = 13;
	GT(1);
L 13:	R5 = R7 + 8;
	R7 = R7 + 12;
	R7 = R7 + 0;
	R0 = P(R5);
	R7 = R7 - 4;
	P(R7 + 0) = R0;
	R7 = R7 - 12;
	P(R7+4) = R6;
	P(R7) = 14;
	GT(2);
L 14:	R5 = R7 + 8;
	R7 = R7 + 12;
	R0 = P(R7 + 0);
	R7 = R7 + 4;
	R1 = P(R5);
	R2 = I(73613);
	R3 = 1;
	R2 = R2 + R3;
	I(73613) = R2;
	GT(11);
L 12:
	GT(-2);
END