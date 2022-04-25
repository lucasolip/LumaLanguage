%{
#include <stdio.h>
#include <string.h>
#include "analizadorsintactico.tab.h"
int line = 1;
%}

letter [a-zA-ZáéíóúÁÉÍÓÚñÑ]
digit [0-9]
letdig {letter}|{digit}

%%
escribe					    return WRITE;
lee						    return READ;
\r\n|\n					    {line++; return NEWINSTRUCTION;}
devuelve					return RETURN;
mientras					return WHILE;
si						    return IF;
sino						return ELSE;
fin						    return END;
[\+\-\*\/yo\(\):,=]			return yytext[0];
modulo						return MODULO;
no						    return NOT;
mayor						return GREATER;
menor						return LESS;
igual						return EQUALS;
distinto					return DIFFERENT;
mayorigual					return GREATEREQUAL;
menorigual					return LESSEQUAL;
Verdadero|verdadero|V		return VERDADERO;
Falso|falso|F			    return FALSO;
{digit}+\.{digit}+			{yylval.floating=atof(yytext);return FLOAT;};
{digit}+					{yylval.integer=atoi(yytext);return INTEGER;};
\'.\'						{yylval.character=yytext[1];return CHARACTER;};
\".*\"|“.*”					{yylval.string=(char*)strdup(yytext);return STRING;};
{letter}{letdig}*			{yylval.identifier=(char*)strdup(yytext);return IDENTIFIER;};
(\/\/.*\n)|(\/\*(.|\n)*\*\/)|[ \t]		// ignore
.						yyerror("Caracter extraño\n");
<<EOF>>					return 0;
%%
