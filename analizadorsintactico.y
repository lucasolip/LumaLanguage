%{
#include <stdio.h>
extern FILE *yyin;   /* declarado en lexico */
extern int line;   /* lexico le da valores */
int yydebug=1;       /* modo debug si -t */
void yyerror(char*); 
%}
%token WRITE READ RETURN WHILE IF ELSE END MODULO NOT VERDADERO FALSO FLOAT INTEGER CHARACTER STRING IDENTIFIER NEWINSTRUCTION GREATER LESS EQUALS GREATEREQUAL LESSEQUAL DIFFERENT
%start program

%left '+' 
%left '-' 
%left '*' 
%left '/'
%left EQUALS
%left DIFFERENT
%left GREATER
%left LESS
%left GREATEREQUAL
%left LESSEQUAL
%left NOT
%left 'y'
%left 'o'

%%
program : instructions;
instructions : instruction | instructions NEWINSTRUCTION instruction;
instruction : 	assignment {printf("Asignación\n");} | 
		control {printf("Condición\n");} | 
		loop {printf("Bucle\n");} | 
		functionDefinition {printf("Definición de función\n");} | 
		functionCall {printf("Llamada a función\n");} | 
		/* Instrucción vacía */ | 
		return;
return : RETURN expression;
assignment : IDENTIFIER '=' expression;
functionCall : WRITE expression | variable;
functionDefinition : variable ':' instructions END | 
			variable ':' instructions error {printf("en definición de función ¿Falta un fin?");};
variable : IDENTIFIER | IDENTIFIER'(' parameter ')' |
		IDENTIFIER'(' parameter error {printf("en función ¿Falta un paréntesis?");};
parameter : expression | parameter ',' expression;
control : IF expression ':' instructions ELSE ':' instructions END | IF expression ':' instructions END | 
		IF expression ':' instructions error {printf("en condición ¿Falta un fin?");} |
		IF expression ':' instructions ELSE ':' instructions error {printf("en condición ¿Falta un fin?");} |
		IF expression error {printf("en condición ¿Faltan los dos puntos?");} instructions END | 
		IF expression ':' instructions ELSE error {printf("en condición ¿Faltan los dos puntos?");} instructions END;
loop : WHILE expression ':' instructions END | 
	WHILE expression ':' instructions error {printf("en bucle ¿Falta un fin?");} |
	WHILE expression error {printf("en bucle ¿Faltan los dos puntos?");} instructions END ;
expression : expression EQUALS expression | expression DIFFERENT expression | expression GREATER expression | expression LESS expression | expression GREATEREQUAL expression | expression LESSEQUAL expression | proposition;
proposition : proposition 'o' andProposition | andProposition;
andProposition : andProposition 'y' negatedProposition | negatedProposition;
negatedProposition : NOT operation | operation;
operation : operation '+' product | operation '-' product | product;
product : product '*' signedOperand | product '/' signedOperand | product MODULO signedOperand | signedOperand;
signedOperand : '-' operand | '+' operand | operand | error {printf("en expresión ¿Falta un operando?");};
operand : FLOAT | INTEGER | STRING | CHARACTER | variable | READ | VERDADERO | FALSO | '(' expression ')' | '(' expression error {printf("en expresión ¿Falta un paréntesis?");};
%%

int main(int argc, char** argv) {
  if (argc>1) yyin=fopen(argv[1],"r");
  yyparse();
}

void yyerror(char* mens) {
  printf("Error en linea %i: %s\n",line,mens);
}
