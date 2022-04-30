%{
#include <stdio.h>
#include "symtable.c"
extern FILE *yyin;   /* declarado en lexico */
extern int line;   /* lexico le da valores */
int yydebug=1;       /* modo debug si -t */
int argumentCount = 0; // Variable auxiliar para contar argumentos de llamadas a funciones 
void yyerror(char*); 

void checkExists(char* identifier) {
	if (!contains(identifier)) {
		char* errorMessage = (char*)malloc(128 * sizeof(char));
		sprintf(errorMessage, "En acceso a variable: La variable %s no existe", identifier);
		yyerror(errorMessage);
		free(errorMessage);
	}
}

void checkArgumentCount(char* functionName) {
	int expectedArguments = getNumArguments(functionName);
	if (expectedArguments == -1) {
		char* errorMessage = (char*)malloc(128 * sizeof(char));
		sprintf(errorMessage, "En llamada a función: La función %s no existe", functionName);
		yyerror(errorMessage);
		free(errorMessage);
		argumentCount = 0;
		return;
	}
	if (argumentCount != expectedArguments) {
		char* errorMessage = (char*)malloc(128 * sizeof(char));
		sprintf(errorMessage, "En función %s: Se dio %d argumentos y se esperaban %d", functionName, argumentCount, expectedArguments);
		yyerror(errorMessage);
		free(errorMessage);
	}
	argumentCount = 0;
}
%}

%union { char* identifier; float floating; char character; int integer; char* string; }
%token WRITE READ RETURN WHILE IF ELSE END MODULO NOT VERDADERO FALSO NEWINSTRUCTION GREATER LESS EQUALS GREATEREQUAL LESSEQUAL DIFFERENT
%token <identifier> IDENTIFIER
%token <floating> FLOAT
%token <character> CHARACTER
%token <integer> INTEGER
%token <string> STRING

%type <identifier> definingParameter
%type <identifier> functionHeader
%type <identifier> functionHeaderOpen
%type <identifier> functionCall

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
		//functionCall {printf("Llamada a función\n");} | 
		writeCall {printf("Llamada a función escribe\n");} |
		bareFunctionCall {printf("Llamada a función sin parámetros\n");} |
		/* Instrucción vacía */ | 
		return {printf("Devuelve\n");};
return : RETURN expression;
assignment : IDENTIFIER '=' expression {insert($1, Unknown); printf("Declaración de variable %s\n", $1);};
bareFunctionCall : IDENTIFIER {checkArgumentCount($1);} | IDENTIFIER '(' ')' {checkArgumentCount($1);};
functionCall : functionHeaderOpen parameter ')' {$$ = $1; popContext(); checkArgumentCount($1);} | 
			functionHeaderOpen parameter error {$$ = $1; popContext(); printf("en llamada a función ¿Falta un paréntesis?\n"); checkArgumentCount($1);};
writeCall : WRITE expression;
functionHeaderOpen : IDENTIFIER '(' {$$ = $1; insert($1, Function); pushContext();};
functionHeader : 	IDENTIFIER ':' {$$ = $1; insert($1, Function); pushContext(); lastFunctionArguments(currentContextVariables());} | 
					functionHeaderOpen definingParameter ')' ':' {$$ = $1; lastFunctionArguments(currentContextVariables());};
functionDefinition : functionHeader instructions END {popContext(); printf("Definición de función %s\n", $1);} | 
			functionHeader instructions error {printf("en definición de función ¿Falta un fin?\n");};
definingParameter : definingParameter ',' IDENTIFIER {$$ = $1; insert($3, Unknown);} | IDENTIFIER {$$ = $1; insert($1, Unknown);};
parameter : expression {argumentCount++;} | parameter ',' expression {argumentCount++;};
controlHeader : IF expression ':' {pushContext();};
control : controlHeader instructions ELSE ':' instructions END {popContext();} | controlHeader instructions END {popContext();} | 
		controlHeader instructions error {printf("en condición ¿Falta un fin?\n");} |
		controlHeader instructions ELSE ':' instructions error {printf("en condición ¿Falta un fin?\n");} |
		IF expression error {printf("en condición ¿Faltan los dos puntos?\n");} instructions END | 
		controlHeader instructions ELSE error {printf("en condición ¿Faltan los dos puntos?\n");} instructions END;
loopHeader : WHILE expression ':' {pushContext();};
loop : loopHeader instructions END {popContext();} | 
	loopHeader instructions error {popContext(); printf("en bucle ¿Falta un fin?\n");} |
	WHILE expression error {printf("en bucle ¿Faltan los dos puntos?\n");} instructions END ;
expression : expression EQUALS expression | expression DIFFERENT expression | expression GREATER expression | expression LESS expression | expression GREATEREQUAL expression | expression LESSEQUAL expression | proposition;
proposition : proposition 'o' andProposition | andProposition;
andProposition : andProposition 'y' negatedProposition | negatedProposition;
negatedProposition : NOT operation | operation;
operation : operation '+' product | operation '-' product | product;
product : product '*' signedOperand | product '/' signedOperand | product MODULO signedOperand | signedOperand;
signedOperand : '-' operand | '+' operand | operand | error {printf("en expresión ¿Falta un operando?\n");};
operand : FLOAT | INTEGER | STRING | CHARACTER | IDENTIFIER {checkExists($1);} |
		functionCall | READ | VERDADERO | FALSO | '(' expression ')' | 
		'(' expression error {printf("en expresión ¿Falta un paréntesis?\n");};
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
