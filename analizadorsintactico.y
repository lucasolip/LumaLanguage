%{
#include <stdio.h>
#include "symtable.c"
extern FILE *yyin;   /* declarado en lexico */
extern int line;   /* lexico le da valores */
int yydebug=1;       /* modo debug si -t */
void yyerror(char*); 
%}

%union { char* identifier; float floating; char character; int integer; char* string; }
%token WRITE READ RETURN WHILE IF ELSE END MODULO NOT VERDADERO FALSO NEWINSTRUCTION GREATER LESS EQUALS GREATEREQUAL LESSEQUAL DIFFERENT
%token <identifier> IDENTIFIER
%token <floating> FLOAT
%token <character> CHARACTER
%token <integer> INTEGER
%token <string> STRING

%type <identifier> variable
%type <identifier> functionHeader
%type <identifier> functionHeaderOpen

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
		functionDefinition | 
		functionCall {printf("Llamada a función\n");} | 
		/* Instrucción vacía */ | 
		return;
return : RETURN expression;
assignment : IDENTIFIER '=' expression {insert($1, Unknown); printf("Declaración de variable %s\n", $1);};
functionCall : WRITE expression | variable;
functionHeaderOpen : IDENTIFIER '(' {$$ = $1; insert($1, Function); pushContext();};
functionHeader : 	IDENTIFIER ':' {$$ = $1; insert($1, Function); pushContext();} | 
					functionHeaderOpen parameter ')' ':' {$$ = $1; lastFunctionArguments(currentContextVariables());};
functionDefinition : functionHeader instructions END {popContext(); printf("Definición de función %s\n", $1);} | 
			functionHeader instructions error {printf("en definición de función ¿Falta un fin?");};
variable : 	IDENTIFIER {$$ = $1; insert($1, Unknown);} | 
			functionHeaderOpen parameter ')' {$$ = $1; popContext(); pop(1);} | 
			functionHeaderOpen parameter error {$$ = $1; popContext(); pop(1); printf("en llamada a función ¿Falta un paréntesis?");};
//definingParameter : definingParameter ',' variable {$$ = $1;} | variable {$$ = $1;};
parameter : expression | parameter ',' expression;
controlHeader : IF expression ':' {pushContext();};
control : controlHeader instructions ELSE ':' instructions END {popContext();} | controlHeader instructions END {popContext();} | 
		controlHeader instructions error {printf("en condición ¿Falta un fin?");} |
		controlHeader instructions ELSE ':' instructions error {printf("en condición ¿Falta un fin?");} |
		IF expression error {printf("en condición ¿Faltan los dos puntos?");} instructions END | 
		controlHeader instructions ELSE error {printf("en condición ¿Faltan los dos puntos?");} instructions END;
loopHeader : WHILE expression ':' {pushContext();};
loop : loopHeader instructions END {popContext();} | 
	loopHeader instructions error {popContext(); printf("en bucle ¿Falta un fin?");} |
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
  printf("\n");
  printStack();
}

void yyerror(char* mens) {
  printf("Error en linea %i: %s\n",line,mens);
}
