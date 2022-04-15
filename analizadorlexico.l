%{
#include <stdio.h>
#include "analizadorsintactico.tab.h"
int line = 1;
%}

letter [a-zA-ZáéíóúÁÉÍÓÚñÑ]
digit [0-9]
letdig {letter}|{digit}

%%
escribe					return WRITE;
lee						return READ;
\r\n|\n					{line++; return NEWINSTRUCTION;}
devuelve					return RETURN;
mientras					return WHILE;
si						return IF;
sino						return ELSE;
fin						return END;
[\+\-\*\/yo\(\):,=]				return yytext[0];
modulo						return MODULO;
no						return NOT;
mayor						return GREATER;
menor						return LESS;
igual						return EQUALS;
distinto					return DIFFERENT;
mayorigual					return GREATEREQUAL;
menorigual					return LESSEQUAL;
verdadero|V					return VERDADERO;
falso|F					return FALSO;
{digit}+\.{digit}+				return FLOAT;
{digit}+					return INTEGER;
\'.\'						return CHARACTER;
\".*\"|“.*”					return STRING;
{letter}{letdig}*				return IDENTIFIER;
(\/\/.*\n)|(\/\*(.|\n)*\*\/)|[ \t]		// ignore
.						yyerror("Caracter extraño\n");
<<EOF>>					return 0;
%%
