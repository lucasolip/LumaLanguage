/* A Bison parser, made by GNU Bison 3.5.1.  */

/* Bison implementation for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2020 Free Software Foundation,
   Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Undocumented macros, especially those whose name start with YY_,
   are private implementation details.  Do not rely on them.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "3.5.1"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* First part of user prologue.  */
#line 1 "analizadorsintactico.y"

#include <stdio.h>
#include <string.h>
#include "symtable.c"
#include "textList.c"
#include "indexStack.c"

struct RegisterNode {
    int* intRegisters;
	int* floatRegisters;
    struct RegisterNode* next;
};

extern FILE *yyin;   /* declarado en lexico */
extern int line;   /* lexico le da valores */
int yydebug=1;       /* modo debug si -t */
int argumentCount = 0; // Variable auxiliar para contar argumentos de llamadas a funciones
int *intRegisters;
int *floatRegisters;
int baseAddress = 73728;
int label = 1;
int localVariablesSize = 0;
int argumentVariablesSize = 0;
struct ListNode* instructionsList = NULL;
struct IndexNode* ifStack;
struct IndexNode* whileStack;
struct RegisterNode* topRegisters = NULL;



void insertRegisters() {
	struct RegisterNode *newNode = (struct RegisterNode *)malloc(sizeof(struct RegisterNode));
    newNode->intRegisters = (int *)malloc(sizeof(int)*7);
	intRegisters = newNode->intRegisters;
	for (int i = 0; i < 8; i++) {
		intRegisters[i] = 0;
	}
	newNode->floatRegisters = (int *)malloc(sizeof(int)*4);
	floatRegisters = newNode->floatRegisters;
	for (int i = 0; i < 4; i++) {
		floatRegisters[i] = 0;
	}
    newNode->next = topRegisters;
    topRegisters = newNode;
}

void removeRegisters() {
	free(topRegisters->intRegisters);
	free(topRegisters->floatRegisters);
    free(topRegisters);
    topRegisters = topRegisters->next;
	intRegisters = topRegisters->intRegisters;
	floatRegisters = topRegisters->floatRegisters;
}

void yyerror(char*);

void pushRegisters() {
	int stackSize = 0;
	for (int i = 0; i < 5; i++) {
		if (intRegisters[i] == 1) stackSize++;
	}
	for (int i = 0; i < 4; i++) {
		if (floatRegisters[i] == 1) stackSize++;
	}
	char line[32];
	snprintf(line, sizeof(line), "\tR7 = R7 - %d;\n", stackSize*4);
	addText(line, &instructionsList);
	int stackIndex = 0;
	for (int i = 0; i < 5; i++) {
		if (intRegisters[i] == 1) {
			snprintf(line, sizeof(line), "\tP(R7 + %d) = R%d;\n", stackIndex*4, i);
			addText(line, &instructionsList);
			stackIndex++;
		}
	}
	for (int i = 0; i < 5; i++) {
		if (floatRegisters[i] == 1) {
			snprintf(line, sizeof(line), "\tP(R7 + %d) = RR%d;\n", stackIndex*4, i);
			addText(line, &instructionsList);
			stackIndex++;
		}
	}
	insertRegisters();
}

void popRegisters() {
	removeRegisters();
	int stackSize = 0;
	for (int i = 0; i < 5; i++) {
		if (intRegisters[i] == 1) stackSize++;
	}
	for (int i = 0; i < 4; i++) {
		if (floatRegisters[i] == 1) stackSize++;
	}
	char line[32];
	
	int stackIndex = 0;
	for (int i = 0; i < 5; i++) {
		if (intRegisters[i] == 1) {
			snprintf(line, sizeof(line), "\tR%d = P(R7 + %d);\n", i, stackIndex*4);
			addText(line, &instructionsList);
			stackIndex++;
		}
	}
	for (int i = 0; i < 5; i++) {
		if (floatRegisters[i] == 1) {
			snprintf(line, sizeof(line), "\tRR%d = P(R7 + %d);\n", i, stackIndex*4);
			addText(line, &instructionsList);
			stackIndex++;
		}
	}
	snprintf(line, sizeof(line), "\tR7 = R7 + %d;\n", stackSize*4);
	addText(line, &instructionsList);
	
}

int getRegisterInt() {
	for (int i = 0; i < 7; i++) {
		if (intRegisters[i] == 0) {
			intRegisters[i] = 1;
			return i;
		}
	}
	printf("Nos quedamos sin registros enteros\n");
	return 0;
}

void freeRegisterInt(int reg) {
	intRegisters[reg] = 0;
}

int getRegisterFloat() {
	for (int i = 0; i < 7; i++) {
		if (floatRegisters[i] == 0) {
			floatRegisters[i] = 1;
			return i;
		}
	}
	printf("Nos quedamos sin registros flotantes\n");
	return 0;
}

void freeRegisterFloat(int reg) {
	floatRegisters[reg] = 0;
}

int getAddress(struct Variable v) {
	switch (v.type) {
		case Integer:
			baseAddress -= 4;
			break;
		case Character:
			baseAddress -= 1;
			break;
		case Float:
			baseAddress -= 4;
			break;
		case Bool:
			baseAddress -= 1;
			break;
		default:
			baseAddress -= 4;
			break;
	}
	return baseAddress;
}

int getLabel() {
	return label++;
}

enum Type getType(char* typeString) {
	if (strcmp(typeString, "entero") == 0) return Integer;
	else if (strcmp(typeString, "decimal") == 0) return Float;
	else if (strcmp(typeString, "caracter") == 0) return Character;
	else if (strcmp(typeString, "booleano") == 0) return Bool;
	else return Void;
}

char getTypeLetter(enum Type type) {
	char regType = 'P';
	switch (type) {
		case Integer:
			regType = 'I';
			break;
		case Character:
			regType = 'U';
			break;
		case Float:
			regType = 'F';
			break;
		case Bool:
			regType = 'U';
			break;
	}
	return regType;
}

void writeStaticZone() {
	if (!staticZoneWritten) {
		staticZoneWritten = 1;
		addTextAtPos("STAT(0)\n", 2, &instructionsList);
		addTextAtPos("CODE(0)\n", 3, &instructionsList);
		staticPosition = 2;
		functionPosition += 2;
		beginningFunction += 2;
	}
	staticPosition++;
	functionPosition++;
	beginningFunction++;
}

struct Variable writeRegisterInt(int immediate) {
	struct Variable reg;
	reg.reg = getRegisterInt();
	reg.type = Integer;
	char line[32];
	snprintf(line, sizeof(line), "\tR%d = %d;\n", reg.reg, immediate);
	addText(line, &instructionsList);
	return reg;
}

struct Variable writeRegisterChar(char immediate) {
	struct Variable reg;
	reg.reg = getRegisterInt();
	reg.type = Character;
	char line[32];
	snprintf(line, sizeof(line), "\tR%d = %c;\n", reg.reg, immediate);
	addText(line, &instructionsList);
	return reg;
}

struct Variable writeRegisterBool(int immediate) {
	struct Variable reg;
	reg.reg = getRegisterInt();
	reg.type = Bool;
	char line[32];
	snprintf(line, sizeof(line), "\tR%d = %d;\n", reg.reg, immediate);
	addText(line, &instructionsList);
	return reg;
}

struct Variable writeRegisterFloat(float immediate) {
	struct Variable reg;
	reg.reg = getRegisterFloat();
	reg.type = Float;
	char line[32];
	snprintf(line, sizeof(line), "\tRR%d = %f;\n", reg.reg, immediate);
	addText(line, &instructionsList);
	return reg;
}

void writeBeforeRegister(struct Variable reg, char operation) {
	if (operation == '~' && reg.type != Bool) {
		yyerror("Operación inválida para tipo no booleano");
		exit(1);
	}
	if (operation == '-' && !(reg.type == Integer || reg.type == Float)) {
		yyerror("Operación inválida para tipo no numérico");
		exit(1);
	}
	char line[32];
	snprintf(line, sizeof(line), "\tR%d = %cR%d;\n", reg.reg, operation, reg.reg);
	addText(line, &instructionsList);
}

struct Variable writeOperation(struct Variable leftReg, struct Variable rightReg, char* operation) {
	char line[32];
	int isBooleanOp = strcmp(operation, "&") == 0 || strcmp(operation, "|") == 0;
	int isComparison = strcmp(operation, "<") == 0 || strcmp(operation, ">") == 0 || 
						strcmp(operation, "==") == 0 || strcmp(operation, "!=") == 0 || 
						strcmp(operation, "<=") == 0 || strcmp(operation, ">=") == 0;
	// Si hay algún caracter
	if (leftReg.type == Character || rightReg.type == Character) {
		yyerror("El tipo caracter no se puede operar");
		exit(1);
	}
	// Si hay algún booleano
	if (leftReg.type == Bool || rightReg.type == Bool) {
		if (isBooleanOp || isComparison) {
			snprintf(line, sizeof(line), "\tR%d = R%d %s R%d;\n", leftReg.reg, leftReg.reg, operation, rightReg.reg);
			freeRegisterInt(rightReg.reg);
			addText(line, &instructionsList);
			return leftReg;
		} else {
			yyerror("Operación inválida para el tipo booleano");
			exit(1);
		}
	}
	// En cualquier otro caso, son numéricos
	if (isBooleanOp) {
		yyerror("Operación inválida para un tipo numérico");
		exit(1);
	}
	if (strcmp(operation, "%") == 0 && (leftReg.type != Integer || rightReg.type != Integer)) {
		yyerror("Operación inválida para el tipo decimal");
		exit(1);
	}
	// Si los dos son del mismo tipo
	if (leftReg.type == rightReg.type) {
		if (leftReg.type == Integer) {
			snprintf(line, sizeof(line), "\tR%d = R%d %s R%d;\n", leftReg.reg, leftReg.reg, operation, rightReg.reg);
			freeRegisterInt(rightReg.reg);
			addText(line, &instructionsList);
			if (isComparison) leftReg.type = Bool;
			return leftReg;
		} else if (leftReg.type == Float) {
			if (isComparison) {
				freeRegisterFloat(leftReg.reg);
				int newBoolean = getRegisterInt();
				snprintf(line, sizeof(line), "\tR%d = RR%d %s RR%d;\n", newBoolean, leftReg.reg, operation, rightReg.reg);
				freeRegisterFloat(leftReg.reg);
				freeRegisterFloat(rightReg.reg);
				leftReg.type = Bool;
				leftReg.reg = newBoolean;
				addText(line, &instructionsList);
				return leftReg;
			} else {
				snprintf(line, sizeof(line), "\tRR%d = RR%d %s RR%d;\n", leftReg.reg, leftReg.reg, operation, rightReg.reg);
				freeRegisterFloat(rightReg.reg);
				addText(line, &instructionsList);
				return leftReg;
			}
		}
	} else if (leftReg.type == Float) {
		if (isComparison) {
			rightReg.type = Bool;
			snprintf(line, sizeof(line), "\tR%d = RR%d %s R%d;\n", rightReg.reg, leftReg.reg, operation, rightReg.reg);
			freeRegisterFloat(leftReg.reg);
			addText(line, &instructionsList);
			return rightReg;
		} else {
			snprintf(line, sizeof(line), "\tRR%d = RR%d %s R%d;\n", leftReg.reg, leftReg.reg, operation, rightReg.reg);
			freeRegisterInt(rightReg.reg);
			addText(line, &instructionsList);
			return leftReg;
		}
		
	} else if (rightReg.type == Float) {
		if (isComparison) {
			leftReg.type = Bool;
			snprintf(line, sizeof(line), "\tR%d = R%d %s RR%d;\n", leftReg.reg, leftReg.reg, operation, rightReg.reg);
			freeRegisterFloat(rightReg.reg);
			addText(line, &instructionsList);
			return leftReg;
		} else {
			snprintf(line, sizeof(line), "\tRR%d = R%d %s RR%d;\n", rightReg.reg, leftReg.reg, operation, rightReg.reg);
			freeRegisterInt(leftReg.reg);
			addText(line, &instructionsList);
			return rightReg;
		}
	}
	return leftReg;
}

void writeAssignment(char* varName, struct Variable v) {
	if (v.type == Void) {
		printf("Aviso: El tipo que se desea asignar está vacío, no se asignará nada.\n");
		return;
	}
	struct Node* var = find(varName);
	int address;
	char regType = getTypeLetter(v.type);
	char line[32];
	if (var != NULL) {
		address = var->address;
		if (var->fromFunction) {
			if (v.type == Float) {
				snprintf(line, sizeof(line), "\t%c(R6 + %d) = RR%d;\n", regType, address, v.reg);
				freeRegisterFloat(v.reg);
			} else {
				snprintf(line, sizeof(line), "\t%c(R6 + %d) = R%d;\n", regType, address, v.reg);
				freeRegisterInt(v.reg);
			}
		} else {
			if (v.type == Float) {
				snprintf(line, sizeof(line), "\t%c(%d) = RR%d;\n", regType, address, v.reg);
				freeRegisterFloat(v.reg);
			} else {
				snprintf(line, sizeof(line), "\t%c(%d) = R%d;\n", regType, address, v.reg);
				freeRegisterInt(v.reg);
			}
		}
	} else {
		if (inFunction) {
			localVariablesSize++;
			address = -localVariablesSize*4;
			insert(varName, v.type, address, inFunction);
			if (v.type == Float) {
				snprintf(line, sizeof(line), "\t%c(R6 + %d) = RR%d;\n", regType, address, v.reg);
				freeRegisterFloat(v.reg);
			} else {
				snprintf(line, sizeof(line), "\t%c(R6 + %d) = R%d;\n", regType, address, v.reg);
				freeRegisterInt(v.reg);
			}
		} else {
			address = getAddress(v);
			insert(varName, v.type, address, inFunction);
			writeStaticZone();
			snprintf(line, sizeof(line), (v.type == Float)? "\tDAT(%d, %c, 0.0);\n" : "\tDAT(%d, %c, 0);\n", address, regType);
			addTextAtPos(line, staticPosition, &instructionsList);
			if (v.type == Float) {
				snprintf(line, sizeof(line), "\t%c(%d) = RR%d;\n", regType, address, v.reg);
				freeRegisterFloat(v.reg);
			} else {
				snprintf(line, sizeof(line), "\t%c(%d) = R%d;\n", regType, address, v.reg);
				freeRegisterInt(v.reg);
			}
		}
	}
	addText(line, &instructionsList);
	
}

void beginFunction(int label) {
	inFunction = 1;
	beginningFunction = functionPosition;
	char line[32];
	snprintf(line, sizeof(line), "L %d:\n", label);
	addText(line, &instructionsList);
	snprintf(line, sizeof(line), "\tR6 = R7;\n");
	addText(line, &instructionsList);
}

void endFunction() {
	char line[32];
	addText("\tR7 = R6;\n", &instructionsList);
	addText("\tR6 = P(R7 + 4);\n", &instructionsList);
	addText("\tR5 = P(R7);\n", &instructionsList);
	addText("\tGT(R5);\n", &instructionsList);
	snprintf(line, sizeof(line), "\tR7 = R7 - %d;\n", localVariablesSize*4);
	addTextAtPos(line, beginningFunction+2, &instructionsList);
	inFunction = 0;
}

void beginCallFunction(struct Node* function) {
	char line[32];
	pushRegisters();
	snprintf(line, sizeof(line), "\tR7 = R7 - %d;\n", (function->numArguments+3) * 4);
	addText(line, &instructionsList);
}

void addCallFunctionParameter(struct Variable v) {
	char line[32];
	char letter = getTypeLetter(v.type);
	char* preregister = (v.type == Float)? "RR" : "R";
	snprintf(line, sizeof(line), "\t%c(R7 + %d) = %s%d;\n", letter, (argumentCount+2)*4, preregister, v.reg);
	addText(line, &instructionsList);
	if (v.type == Float) freeRegisterFloat(v.reg);
	else freeRegisterInt(v.reg);
}

struct Variable callFunction(struct Node* function) {
	struct Variable v;
	v.type = function->returnType;
	int address = function->address;
	int label = getLabel();
	char line[32];
	addText("\tP(R7+4) = R6;\n", &instructionsList);
	snprintf(line, sizeof(line), "\tP(R7) = %d;\n", label);
	addText(line, &instructionsList);
	snprintf(line, sizeof(line), "\tGT(%d);\n", address);
	addText(line, &instructionsList);
	char letter = getTypeLetter(v.type);
	snprintf(line, sizeof(line), "L %d:\tR5 = R7 + 8;\n", label);
	addText(line, &instructionsList);
	snprintf(line, sizeof(line), "\tR7 = R7 + %d;\n", (function->numArguments+3) * 4);
	addText(line, &instructionsList);
	popRegisters();
	if (v.type == Float) {
		int reg = getRegisterFloat();
		snprintf(line, sizeof(line), "\tRR%d = %c(R5);\n", reg, letter);
		v.reg = reg;
	} else {
		int reg = getRegisterInt();
		snprintf(line, sizeof(line), "\tR%d = %c(R5);\n", reg, letter);
		v.reg = reg;
	}
	addText(line, &instructionsList);
	return v;
}

struct Variable writeVariableAccess(char* varName) {
	struct Node* var = find(varName);
	if (var->type == Function) {
		return callFunction(var);
	}
	struct Variable v;
	char regType = getTypeLetter(var->type);
	char line[32];
	v.type = var->type;
	int address = var->address;
	if (var->fromFunction) {
		if (v.type == Float) {
			v.reg = getRegisterFloat();
			snprintf(line, sizeof(line), "\tRR%d = %c(R6 + %d);\n", v.reg, regType, address);
		} else {
			v.reg = getRegisterInt();
			snprintf(line, sizeof(line), "\tR%d = %c(R6 + %d);\n", v.reg, regType, address);
		}
	} else {
		if (v.type == Float) {
			v.reg = getRegisterFloat();
			snprintf(line, sizeof(line), "\tRR%d = %c(%d);\n", v.reg, regType, address);
		} else {
			v.reg = getRegisterInt();
			snprintf(line, sizeof(line), "\tR%d = %c(%d);\n", v.reg, regType, address);
		}
	}
	addText(line, &instructionsList);
	return v;
}

void writePrint(struct Variable v) {
	baseAddress -= 3;
	int address = baseAddress;
	char letter;
	switch (v.type) {
		case Integer:
			letter = 'i';
			break;
		case Float:
			letter = 'f';
			break;
		case Character:
			letter = 'c';
			break;
		default:
			letter = 'i';
			break;
	}
	writeStaticZone();
	char line[32];
	snprintf(line, sizeof(line), "\tSTR(%d, \"%%%c\");\n", address, letter);
	addTextAtPos(line, staticPosition, &instructionsList);
	int label = getLabel();
	if (v.type == Float) freeRegisterFloat(v.reg);
	else freeRegisterInt(v.reg);
	snprintf(line, sizeof(line), (v.type == Float)? "\tRR3 = RR%d;\n" : "\tR3 = R%d;\n", v.reg);
	addText(line, &instructionsList);
	snprintf(line, sizeof(line), "\tR0 = %d;\n", label);
	addText(line, &instructionsList);
	snprintf(line, sizeof(line), "\tR1 = %d;\n", address);
	addText(line, &instructionsList);
	addText((v.type == Float)? "\tRR2 = RR3;\n" : "\tR2 = R3;\n", &instructionsList);
	addText((v.type == Float)? "\tGT(putfloat_);\n" : "\tGT(putf_);\n", &instructionsList);
	snprintf(line, sizeof(line), "L %d:\n", label);
	addText(line, &instructionsList);
}

void writePrintString(char* string) {
	int size = strlen(string);
	baseAddress -= strlen(string);
	int address = baseAddress;
	writeStaticZone();
	char line[32+size];
	snprintf(line, sizeof(line), "\tSTR(%d, %s);\n", address, string);
	addTextAtPos(line, staticPosition, &instructionsList);
	int label = getLabel();
	snprintf(line, sizeof(line), "\tR0 = %d;\n", label);
	addText(line, &instructionsList);
	snprintf(line, sizeof(line), "\tR1 = %d;\n", address);
	addText(line, &instructionsList);
	addText("\tGT(putf_);\n", &instructionsList);
	snprintf(line, sizeof(line), "L %d:\n", label);
	addText(line, &instructionsList);
}

void writeConditionalStart(struct Variable condition) {
	if (condition.type != Bool) {
		yyerror("Las condiciones deben ser booleanas\n");
		exit(1);
	}
	freeRegisterInt(condition.reg);
	char line[32];
	insertIndex(getLabel(), &ifStack);
	snprintf(line, sizeof(line), "\tIF (!R%d) GT(%d);\n", condition.reg, getIndex(&ifStack));
	addText(line, &instructionsList);
}

void writeConditionalEnd() {
	char line[32];
	snprintf(line, sizeof(line), "L %d:\n", popIndex(&ifStack));
	addText(line, &instructionsList);
}

void writeElse() {
	int label = getLabel();
	char line[32];
	snprintf(line, sizeof(line), "\tGT(%d);\n", label);
	addText(line, &instructionsList);
	snprintf(line, sizeof(line), "L %d:\n", popIndex(&ifStack));
	addText(line, &instructionsList);
	insertIndex(label, &ifStack);
}

void writeWhileLabel() {
	char line[32];
	int startWhileLabel = getLabel();
	snprintf(line, sizeof(line), "L %d:\n", startWhileLabel);
	addText(line, &instructionsList);
	insertIndex(startWhileLabel, &whileStack);
}

void writeWhileStart(struct Variable condition) {
	if (condition.type != Bool) {
		yyerror("Las condiciones deben ser booleanas\n");
		exit(1);
	}
	freeRegisterInt(condition.reg);
	char line[32];
	int endWhileLabel = getLabel();
	snprintf(line, sizeof(line), "\tIF (!R%d) GT(%d);\n", condition.reg, endWhileLabel);
	addText(line, &instructionsList);
	int startWhileLabel = popIndex(&whileStack);
	insertIndex(endWhileLabel, &whileStack);
	insertIndex(startWhileLabel, &whileStack);
}

void writeWhileEnd() {
	char line[32];
	snprintf(line, sizeof(line), "\tGT(%d);\n", popIndex(&whileStack));
	addText(line, &instructionsList);
	snprintf(line, sizeof(line), "L %d:\n", popIndex(&whileStack));
	addText(line, &instructionsList);
	insertIndex(label, &ifStack);
}

void checkExists(char* identifier) {
	if (!contains(identifier)) {
		char* errorMessage = (char*)malloc(128 * sizeof(char));
		sprintf(errorMessage, "En acceso a variable: La variable %s no existe", identifier);
		yyerror(errorMessage);
		free(errorMessage);
		exit(1);
	}
}

void checkArgumentCount(struct Node* function) {
	int expectedArguments = function->numArguments;
	char* functionName = function->name;
	if (expectedArguments == -1) {
		char* errorMessage = (char*)malloc(128 * sizeof(char));
		sprintf(errorMessage, "En llamada a función: La función %s no existe", functionName);
		yyerror(errorMessage);
		free(errorMessage);
		argumentCount = 0;
		exit(1);
		return;
	}
	if (argumentCount != expectedArguments) {
		char* errorMessage = (char*)malloc(128 * sizeof(char));
		sprintf(errorMessage, "En función %s: Se dio %d argumentos y se esperaban %d", functionName, argumentCount, expectedArguments);
		yyerror(errorMessage);
		free(errorMessage);
		exit(1);
	}
	argumentCount = 0;
}

struct Node* distinguishFunctionCall(char* functionName) {
	struct Node* f = find(functionName);
	if (f == NULL) {
		int label = getLabel();
		f = insert(functionName, Function, label, inFunction); 
		pushContext(); 
		beginFunction(label);
	} else {
		beginCallFunction(f);
	}
	return f;
}

void writeReturn(struct Variable v) {
	setFunctionType(v.type);
	char line[32];
	char letter = getTypeLetter(v.type);
	if (v.type == Float) {
		snprintf(line, sizeof(line), "\t%c(R6 + 8) = RR%d;\n", letter, v.reg);
		freeRegisterFloat(v.reg);
	} else {
		snprintf(line, sizeof(line), "\t%c(R6 + 8) = R%d;\n", letter, v.reg);
		freeRegisterInt(v.reg);
	}
	addText(line, &instructionsList);
	addText("\tR7 = R6;\n", &instructionsList);
	addText("\tR6 = P(R7 + 4);\n", &instructionsList);
	addText("\tR5 = P(R7);\n", &instructionsList);
	addText("\tGT(R5);\n", &instructionsList);
}

struct Variable writeRead(char* readType) {
	enum Type type = getType(readType);
	char line[32];
	int tempRegister0, tempRegister1;
	struct Variable v;
	if (type == Integer) {
		if (intRegisters[0] == 1) {
			tempRegister0 = getRegisterInt();
			snprintf(line, sizeof(line), "\tR%d = R0;\n", tempRegister0);
			addText(line, &instructionsList);
		}
		if (intRegisters[1] == 1) {
			tempRegister1 = getRegisterInt();
			snprintf(line, sizeof(line), "\tR%d = R1;\n", tempRegister1);
			addText(line, &instructionsList);
		}
		int label = getLabel();
		snprintf(line, sizeof(line), "\tR0 = %d;\n", label);
		addText(line, &instructionsList);
		addText("\tGT(getInteger_);\n", &instructionsList);
		snprintf(line, sizeof(line), "L %d:\n", label);
		addText(line, &instructionsList);
		if (intRegisters[0] == 1) {
			freeRegisterInt(tempRegister0);
			snprintf(line, sizeof(line), "\tR0 = R%d;\n", tempRegister0);
			addText(line, &instructionsList);
		}
		v.reg = getRegisterInt();
		v.type = Integer;
		snprintf(line, sizeof(line), "\tR%d = R1;\n", v.reg);
		addText(line, &instructionsList);
		if (intRegisters[1] == 1) {
			freeRegisterInt(tempRegister1);
			snprintf(line, sizeof(line), "\tR1 = R%d;\n", tempRegister1);
			addText(line, &instructionsList);
		}
		return v;
	} else if (type == Float) {
		if (intRegisters[0] == 1) {
			tempRegister0 = getRegisterInt();
			snprintf(line, sizeof(line), "\tR%d = R0;\n", tempRegister0);
			addText(line, &instructionsList);
		}
		if (floatRegisters[1] == 1) {
			tempRegister1 = getRegisterFloat();
			snprintf(line, sizeof(line), "\tRR%d = RR1;\n", tempRegister1);
			addText(line, &instructionsList);
		}
		int label = getLabel();
		snprintf(line, sizeof(line), "\tR0 = %d;\n", label);
		addText(line, &instructionsList);
		addText("\tGT(getFloat_);\n", &instructionsList);
		snprintf(line, sizeof(line), "L %d:\n", label);
		addText(line, &instructionsList);
		if (intRegisters[0] == 1) {
			freeRegisterInt(tempRegister0);
			snprintf(line, sizeof(line), "\tR0 = R%d;\n", tempRegister0);
			addText(line, &instructionsList);
		}
		v.reg = getRegisterFloat();
		v.type = Float;
		snprintf(line, sizeof(line), "\tRR%d = RR1;\n", v.reg);
		addText(line, &instructionsList);
		if (floatRegisters[1] == 1) {
			freeRegisterFloat(tempRegister1);
			snprintf(line, sizeof(line), "\tRR1 = RR%d;\n", tempRegister1);
			addText(line, &instructionsList);
		}
		return v;
	}
	yyerror("No es posible leer este tipo de dato");
	exit(0);
}




#line 840 "analizadorsintactico.tab.c"

# ifndef YY_CAST
#  ifdef __cplusplus
#   define YY_CAST(Type, Val) static_cast<Type> (Val)
#   define YY_REINTERPRET_CAST(Type, Val) reinterpret_cast<Type> (Val)
#  else
#   define YY_CAST(Type, Val) ((Type) (Val))
#   define YY_REINTERPRET_CAST(Type, Val) ((Type) (Val))
#  endif
# endif
# ifndef YY_NULLPTR
#  if defined __cplusplus
#   if 201103L <= __cplusplus
#    define YY_NULLPTR nullptr
#   else
#    define YY_NULLPTR 0
#   endif
#  else
#   define YY_NULLPTR ((void*)0)
#  endif
# endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

/* Use api.header.include to #include this header
   instead of duplicating it here.  */
#ifndef YY_YY_ANALIZADORSINTACTICO_TAB_H_INCLUDED
# define YY_YY_ANALIZADORSINTACTICO_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    WRITE = 258,
    READ = 259,
    RETURN = 260,
    WHILE = 261,
    IF = 262,
    ELSE = 263,
    END = 264,
    MODULO = 265,
    NOT = 266,
    VERDADERO = 267,
    FALSO = 268,
    NEWINSTRUCTION = 269,
    GREATER = 270,
    LESS = 271,
    EQUALS = 272,
    GREATEREQUAL = 273,
    LESSEQUAL = 274,
    DIFFERENT = 275,
    IDENTIFIER = 276,
    FLOAT = 277,
    CHARACTER = 278,
    INTEGER = 279,
    STRING = 280,
    TYPE = 281
  };
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 771 "analizadorsintactico.y"
 char* identifier; struct Variable reg; struct Node* symbol; 
			float floating; char character; int integer; char* string; 

#line 923 "analizadorsintactico.tab.c"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_ANALIZADORSINTACTICO_TAB_H_INCLUDED  */



#ifdef short
# undef short
#endif

/* On compilers that do not define __PTRDIFF_MAX__ etc., make sure
   <limits.h> and (if available) <stdint.h> are included
   so that the code can choose integer types of a good width.  */

#ifndef __PTRDIFF_MAX__
# include <limits.h> /* INFRINGES ON USER NAME SPACE */
# if defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stdint.h> /* INFRINGES ON USER NAME SPACE */
#  define YY_STDINT_H
# endif
#endif

/* Narrow types that promote to a signed type and that can represent a
   signed or unsigned integer of at least N bits.  In tables they can
   save space and decrease cache pressure.  Promoting to a signed type
   helps avoid bugs in integer arithmetic.  */

#ifdef __INT_LEAST8_MAX__
typedef __INT_LEAST8_TYPE__ yytype_int8;
#elif defined YY_STDINT_H
typedef int_least8_t yytype_int8;
#else
typedef signed char yytype_int8;
#endif

#ifdef __INT_LEAST16_MAX__
typedef __INT_LEAST16_TYPE__ yytype_int16;
#elif defined YY_STDINT_H
typedef int_least16_t yytype_int16;
#else
typedef short yytype_int16;
#endif

#if defined __UINT_LEAST8_MAX__ && __UINT_LEAST8_MAX__ <= __INT_MAX__
typedef __UINT_LEAST8_TYPE__ yytype_uint8;
#elif (!defined __UINT_LEAST8_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST8_MAX <= INT_MAX)
typedef uint_least8_t yytype_uint8;
#elif !defined __UINT_LEAST8_MAX__ && UCHAR_MAX <= INT_MAX
typedef unsigned char yytype_uint8;
#else
typedef short yytype_uint8;
#endif

#if defined __UINT_LEAST16_MAX__ && __UINT_LEAST16_MAX__ <= __INT_MAX__
typedef __UINT_LEAST16_TYPE__ yytype_uint16;
#elif (!defined __UINT_LEAST16_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST16_MAX <= INT_MAX)
typedef uint_least16_t yytype_uint16;
#elif !defined __UINT_LEAST16_MAX__ && USHRT_MAX <= INT_MAX
typedef unsigned short yytype_uint16;
#else
typedef int yytype_uint16;
#endif

#ifndef YYPTRDIFF_T
# if defined __PTRDIFF_TYPE__ && defined __PTRDIFF_MAX__
#  define YYPTRDIFF_T __PTRDIFF_TYPE__
#  define YYPTRDIFF_MAXIMUM __PTRDIFF_MAX__
# elif defined PTRDIFF_MAX
#  ifndef ptrdiff_t
#   include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  endif
#  define YYPTRDIFF_T ptrdiff_t
#  define YYPTRDIFF_MAXIMUM PTRDIFF_MAX
# else
#  define YYPTRDIFF_T long
#  define YYPTRDIFF_MAXIMUM LONG_MAX
# endif
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned
# endif
#endif

#define YYSIZE_MAXIMUM                                  \
  YY_CAST (YYPTRDIFF_T,                                 \
           (YYPTRDIFF_MAXIMUM < YY_CAST (YYSIZE_T, -1)  \
            ? YYPTRDIFF_MAXIMUM                         \
            : YY_CAST (YYSIZE_T, -1)))

#define YYSIZEOF(X) YY_CAST (YYPTRDIFF_T, sizeof (X))

/* Stored state numbers (used for stacks). */
typedef yytype_uint8 yy_state_t;

/* State numbers in computations.  */
typedef int yy_state_fast_t;

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(Msgid) Msgid
# endif
#endif

#ifndef YY_ATTRIBUTE_PURE
# if defined __GNUC__ && 2 < __GNUC__ + (96 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_PURE __attribute__ ((__pure__))
# else
#  define YY_ATTRIBUTE_PURE
# endif
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# if defined __GNUC__ && 2 < __GNUC__ + (7 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_UNUSED __attribute__ ((__unused__))
# else
#  define YY_ATTRIBUTE_UNUSED
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(E) ((void) (E))
#else
# define YYUSE(E) /* empty */
#endif

#if defined __GNUC__ && ! defined __ICC && 407 <= __GNUC__ * 100 + __GNUC_MINOR__
/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                            \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")              \
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# define YY_IGNORE_MAYBE_UNINITIALIZED_END      \
    _Pragma ("GCC diagnostic pop")
#else
# define YY_INITIAL_VALUE(Value) Value
#endif
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif

#if defined __cplusplus && defined __GNUC__ && ! defined __ICC && 6 <= __GNUC__
# define YY_IGNORE_USELESS_CAST_BEGIN                          \
    _Pragma ("GCC diagnostic push")                            \
    _Pragma ("GCC diagnostic ignored \"-Wuseless-cast\"")
# define YY_IGNORE_USELESS_CAST_END            \
    _Pragma ("GCC diagnostic pop")
#endif
#ifndef YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_END
#endif


#define YY_ASSERT(E) ((void) (0 && (E)))

#if ! defined yyoverflow || YYERROR_VERBOSE

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
      /* Use EXIT_SUCCESS as a witness for stdlib.h.  */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's 'empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
             && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* ! defined yyoverflow || YYERROR_VERBOSE */


#if (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yy_state_t yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (YYSIZEOF (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (YYSIZEOF (yy_state_t) + YYSIZEOF (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)                           \
    do                                                                  \
      {                                                                 \
        YYPTRDIFF_T yynewbytes;                                         \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * YYSIZEOF (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / YYSIZEOF (*yyptr);                        \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, YY_CAST (YYSIZE_T, (Count)) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYPTRDIFF_T yyi;                      \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  50
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   222

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  38
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  31
/* YYNRULES -- Number of rules.  */
#define YYNRULES  81
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  134

#define YYUNDEFTOK  2
#define YYMAXUTOK   281


/* YYTRANSLATE(TOKEN-NUM) -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, with out-of-bounds checking.  */
#define YYTRANSLATE(YYX)                                                \
  (0 <= (YYX) && (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex.  */
static const yytype_int8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
      34,    35,    29,    27,    37,    28,     2,    30,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,    36,     2,
       2,    33,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,    32,     2,     2,     2,     2,     2,     2,     2,     2,
       2,    31,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26
};

#if YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,   813,   813,   814,   814,   815,   816,   817,   818,   819,
     820,   821,   821,   823,   824,   825,   826,   827,   828,   829,
     830,   831,   832,   833,   836,   838,   839,   840,   841,   842,
     843,   844,   845,   846,   847,   848,   849,   850,   850,   851,
     851,   852,   853,   854,   855,   856,   856,   857,   858,   859,
     860,   861,   862,   863,   864,   865,   866,   867,   868,   869,
     870,   871,   872,   873,   874,   875,   876,   877,   878,   879,
     880,   881,   882,   883,   884,   885,   886,   887,   888,   889,
     890,   891
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || 0
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "WRITE", "READ", "RETURN", "WHILE", "IF",
  "ELSE", "END", "MODULO", "NOT", "VERDADERO", "FALSO", "NEWINSTRUCTION",
  "GREATER", "LESS", "EQUALS", "GREATEREQUAL", "LESSEQUAL", "DIFFERENT",
  "IDENTIFIER", "FLOAT", "CHARACTER", "INTEGER", "STRING", "TYPE", "'+'",
  "'-'", "'*'", "'/'", "'y'", "'o'", "'='", "'('", "')'", "':'", "','",
  "$accept", "program", "instructions", "instruction", "return",
  "assignment", "bareFunctionCall", "functionCall", "writeCall",
  "functionHeaderOpen", "functionHeader", "functionDefinition",
  "definingParameter", "parameter", "controlHeader", "elseHeader",
  "control", "$@1", "$@2", "loopKeyword", "loopHeader", "loop", "$@3",
  "expression", "proposition", "andProposition", "negatedProposition",
  "operation", "product", "signedOperand", "operand", YY_NULLPTR
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[NUM] -- (External) token number corresponding to the
   (internal) symbol number NUM (which must be that of a token).  */
static const yytype_int16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,    43,    45,    42,
      47,   121,   111,    61,    40,    41,    58,    44
};
# endif

#define YYPACT_NINF (-56)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-1)

#define yytable_value_is_error(Yyn) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
      38,   107,   160,   -56,   160,    14,    66,     5,   -56,   -56,
     -56,   -56,   -56,   -56,   132,    38,   -56,    38,   -56,   160,
      38,   -56,   -56,    10,   185,   -56,   -56,    70,   -56,   -56,
     -56,   -56,    78,    78,   160,   -56,   160,   184,     6,    74,
     -56,     7,    -4,   -56,   -56,   184,    13,   160,    71,   -56,
     -56,    38,    86,    19,     2,   184,   148,   138,    45,   166,
     -56,   -56,     7,   -56,   -56,   -56,    52,   160,   160,   160,
     160,   160,   160,   160,   160,   185,   185,   185,   185,   185,
     -56,   -56,   184,   -56,   -56,   -56,    73,    88,   -56,   -56,
     160,   -56,   -56,   -56,     4,   -56,    38,   -56,   -56,   -56,
     -56,   -56,   -56,    76,    39,   202,    96,   -56,   177,    74,
     -56,    -4,    -4,   -56,   -56,   -56,    38,   -56,   100,   184,
     -56,   -56,   176,    38,    -1,   -56,    38,   -56,   -56,    75,
     -56,    79,   -56,   -56
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_int8 yydefact[] =
{
      12,     0,     0,    41,     0,    16,     0,     2,     3,    13,
       5,    11,     9,    10,     0,    12,     8,    12,     6,     0,
      12,     7,    70,     0,     0,    78,    79,    74,    71,    73,
      72,    21,     0,     0,     0,    75,     0,    20,    53,    55,
      57,    59,    62,    66,    69,    14,     0,     0,    22,    23,
       1,    12,     0,     0,     0,    29,     0,     0,     0,     0,
      77,    76,    58,    22,    68,    67,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      37,    31,    15,    17,     4,    28,     0,     0,    19,    18,
       0,    26,    25,    35,     0,    34,    12,    45,    42,    44,
      43,    81,    80,    49,    50,    47,    51,    52,    48,    54,
      56,    60,    61,    65,    63,    64,    12,    24,     0,    30,
      39,    32,     0,    12,     0,    27,    12,    36,    33,     0,
      38,     0,    46,    40
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int8 yypgoto[] =
{
     -56,   -56,   -13,    87,   -56,   -56,   -56,     0,   -56,     1,
     -56,   -56,   -56,   -56,   -56,   -56,   -56,   -56,   -56,   -56,
     -56,   -56,   -56,     8,   -56,    49,    51,   113,    -2,   -55,
      53
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int8 yydefgoto[] =
{
      -1,     6,     7,     8,     9,    10,    11,    35,    13,    36,
      15,    16,    53,    54,    17,    96,    18,   116,   126,    19,
      20,    21,   123,    55,    38,    39,    40,    41,    42,    43,
      44
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_uint8 yytable[] =
{
      12,    14,    56,    88,    57,   120,    77,    59,   130,    37,
      45,    60,    46,    51,    80,    12,    14,    12,    14,    51,
      12,    14,   113,   114,   115,    78,    79,    58,    67,    68,
      69,    70,    71,    72,    75,    76,    61,    89,    73,    90,
     121,     1,    66,     2,     3,     4,    97,    47,    48,    81,
      49,    12,    14,   101,    86,    82,    87,    70,    71,     5,
      67,    68,    69,    70,    71,    72,    50,    67,    68,    69,
      70,    71,    72,   111,   112,   103,   104,   105,   106,   107,
     108,    98,    23,   122,   132,    64,    65,   102,   133,    51,
      25,    26,    68,    51,    70,    71,    12,    14,   119,    27,
      28,    29,    30,   124,    63,    74,    83,    85,    22,   117,
     129,    23,    34,   131,   118,    71,    12,    14,    24,    25,
      26,   125,   109,    12,    14,   110,    12,    14,    27,    28,
      29,    30,    31,    22,    32,    33,    23,    62,    84,    93,
       0,    34,     0,    24,    25,    26,    94,    95,     0,    91,
       0,     0,    51,    27,    28,    29,    30,    92,    52,    32,
      33,    22,    51,     0,    23,     0,    34,    99,     0,     0,
       0,    24,    25,    26,     0,   100,     0,   127,     0,     0,
      51,    27,    28,    29,    30,   128,    22,    32,    33,    23,
      51,     0,    67,    68,    34,    70,    71,    25,    26,    67,
      68,    69,    70,    71,    72,     0,    27,    28,    29,    30,
       0,     0,    32,    33,     0,     0,     0,    67,    68,    34,
      70,    71,    72
};

static const yytype_int8 yycheck[] =
{
       0,     0,    15,     1,    17,     1,    10,    20,     9,     1,
       2,     1,     4,    14,     1,    15,    15,    17,    17,    14,
      20,    20,    77,    78,    79,    29,    30,    19,    15,    16,
      17,    18,    19,    20,    27,    28,    26,    35,    32,    37,
      36,     3,    34,     5,     6,     7,     1,    33,    34,    36,
      36,    51,    51,     1,    35,    47,    37,    18,    19,    21,
      15,    16,    17,    18,    19,    20,     0,    15,    16,    17,
      18,    19,    20,    75,    76,    67,    68,    69,    70,    71,
      72,    36,     4,    96,     9,    32,    33,    35,     9,    14,
      12,    13,    16,    14,    18,    19,    96,    96,    90,    21,
      22,    23,    24,   116,    34,    31,    35,    21,     1,    36,
     123,     4,    34,   126,    26,    19,   116,   116,    11,    12,
      13,    21,    73,   123,   123,    74,   126,   126,    21,    22,
      23,    24,    25,     1,    27,    28,     4,    24,    51,     1,
      -1,    34,    -1,    11,    12,    13,     8,     9,    -1,     1,
      -1,    -1,    14,    21,    22,    23,    24,     9,    26,    27,
      28,     1,    14,    -1,     4,    -1,    34,     1,    -1,    -1,
      -1,    11,    12,    13,    -1,     9,    -1,     1,    -1,    -1,
      14,    21,    22,    23,    24,     9,     1,    27,    28,     4,
      14,    -1,    15,    16,    34,    18,    19,    12,    13,    15,
      16,    17,    18,    19,    20,    -1,    21,    22,    23,    24,
      -1,    -1,    27,    28,    -1,    -1,    -1,    15,    16,    34,
      18,    19,    20
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_int8 yystos[] =
{
       0,     3,     5,     6,     7,    21,    39,    40,    41,    42,
      43,    44,    45,    46,    47,    48,    49,    52,    54,    57,
      58,    59,     1,     4,    11,    12,    13,    21,    22,    23,
      24,    25,    27,    28,    34,    45,    47,    61,    62,    63,
      64,    65,    66,    67,    68,    61,    61,    33,    34,    36,
       0,    14,    26,    50,    51,    61,    40,    40,    61,    40,
       1,    26,    65,    34,    68,    68,    61,    15,    16,    17,
      18,    19,    20,    32,    31,    27,    28,    10,    29,    30,
       1,    36,    61,    35,    41,    21,    35,    37,     1,    35,
      37,     1,     9,     1,     8,     9,    53,     1,    36,     1,
       9,     1,    35,    61,    61,    61,    61,    61,    61,    63,
      64,    66,    66,    67,    67,    67,    55,    36,    26,    61,
       1,    36,    40,    60,    40,    21,    56,     1,     9,    40,
       9,    40,     9,     9
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_int8 yyr1[] =
{
       0,    38,    39,    40,    40,    41,    41,    41,    41,    41,
      41,    41,    41,    41,    42,    43,    44,    44,    45,    45,
      46,    46,    47,    48,    48,    49,    49,    50,    50,    51,
      51,    52,    53,    54,    54,    54,    54,    55,    54,    56,
      54,    57,    58,    59,    59,    60,    59,    61,    61,    61,
      61,    61,    61,    61,    62,    62,    63,    63,    64,    64,
      65,    65,    65,    66,    66,    66,    66,    67,    67,    67,
      67,    68,    68,    68,    68,    68,    68,    68,    68,    68,
      68,    68
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     1,     1,     3,     1,     1,     1,     1,     1,
       1,     1,     0,     1,     2,     3,     1,     3,     3,     3,
       2,     2,     2,     2,     4,     3,     3,     4,     2,     1,
       3,     3,     2,     5,     3,     3,     5,     0,     6,     0,
       7,     1,     3,     3,     3,     0,     6,     3,     3,     3,
       3,     3,     3,     1,     3,     1,     3,     1,     2,     1,
       3,     3,     1,     3,     3,     3,     1,     2,     2,     1,
       1,     1,     1,     1,     1,     1,     2,     2,     1,     1,
       3,     3
};


#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)
#define YYEMPTY         (-2)
#define YYEOF           0

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                    \
  do                                                              \
    if (yychar == YYEMPTY)                                        \
      {                                                           \
        yychar = (Token);                                         \
        yylval = (Value);                                         \
        YYPOPSTACK (yylen);                                       \
        yystate = *yyssp;                                         \
        goto yybackup;                                            \
      }                                                           \
    else                                                          \
      {                                                           \
        yyerror (YY_("syntax error: cannot back up")); \
        YYERROR;                                                  \
      }                                                           \
  while (0)

/* Error token number */
#define YYTERROR        1
#define YYERRCODE       256



/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)                        \
do {                                            \
  if (yydebug)                                  \
    YYFPRINTF Args;                             \
} while (0)

/* This macro is provided for backward compatibility. */
#ifndef YY_LOCATION_PRINT
# define YY_LOCATION_PRINT(File, Loc) ((void) 0)
#endif


# define YY_SYMBOL_PRINT(Title, Type, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Type, Value); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*-----------------------------------.
| Print this symbol's value on YYO.  |
`-----------------------------------*/

static void
yy_symbol_value_print (FILE *yyo, int yytype, YYSTYPE const * const yyvaluep)
{
  FILE *yyoutput = yyo;
  YYUSE (yyoutput);
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyo, yytoknum[yytype], *yyvaluep);
# endif
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YYUSE (yytype);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/*---------------------------.
| Print this symbol on YYO.  |
`---------------------------*/

static void
yy_symbol_print (FILE *yyo, int yytype, YYSTYPE const * const yyvaluep)
{
  YYFPRINTF (yyo, "%s %s (",
             yytype < YYNTOKENS ? "token" : "nterm", yytname[yytype]);

  yy_symbol_value_print (yyo, yytype, yyvaluep);
  YYFPRINTF (yyo, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yy_state_t *yybottom, yy_state_t *yytop)
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)                            \
do {                                                            \
  if (yydebug)                                                  \
    yy_stack_print ((Bottom), (Top));                           \
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

static void
yy_reduce_print (yy_state_t *yyssp, YYSTYPE *yyvsp, int yyrule)
{
  int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %d):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       yystos[+yyssp[yyi + 1 - yynrhs]],
                       &yyvsp[(yyi + 1) - (yynrhs)]
                                              );
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, Rule); \
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif


#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined __GLIBC__ && defined _STRING_H
#   define yystrlen(S) (YY_CAST (YYPTRDIFF_T, strlen (S)))
#  else
/* Return the length of YYSTR.  */
static YYPTRDIFF_T
yystrlen (const char *yystr)
{
  YYPTRDIFF_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
static char *
yystpcpy (char *yydest, const char *yysrc)
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

# ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYPTRDIFF_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYPTRDIFF_T yyn = 0;
      char const *yyp = yystr;

      for (;;)
        switch (*++yyp)
          {
          case '\'':
          case ',':
            goto do_not_strip_quotes;

          case '\\':
            if (*++yyp != '\\')
              goto do_not_strip_quotes;
            else
              goto append;

          append:
          default:
            if (yyres)
              yyres[yyn] = *yyp;
            yyn++;
            break;

          case '"':
            if (yyres)
              yyres[yyn] = '\0';
            return yyn;
          }
    do_not_strip_quotes: ;
    }

  if (yyres)
    return yystpcpy (yyres, yystr) - yyres;
  else
    return yystrlen (yystr);
}
# endif

/* Copy into *YYMSG, which is of size *YYMSG_ALLOC, an error message
   about the unexpected token YYTOKEN for the state stack whose top is
   YYSSP.

   Return 0 if *YYMSG was successfully written.  Return 1 if *YYMSG is
   not large enough to hold the message.  In that case, also set
   *YYMSG_ALLOC to the required number of bytes.  Return 2 if the
   required number of bytes is too large to store.  */
static int
yysyntax_error (YYPTRDIFF_T *yymsg_alloc, char **yymsg,
                yy_state_t *yyssp, int yytoken)
{
  enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
  /* Internationalized format string. */
  const char *yyformat = YY_NULLPTR;
  /* Arguments of yyformat: reported tokens (one for the "unexpected",
     one per "expected"). */
  char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
  /* Actual size of YYARG. */
  int yycount = 0;
  /* Cumulated lengths of YYARG.  */
  YYPTRDIFF_T yysize = 0;

  /* There are many possibilities here to consider:
     - If this state is a consistent state with a default action, then
       the only way this function was invoked is if the default action
       is an error action.  In that case, don't check for expected
       tokens because there are none.
     - The only way there can be no lookahead present (in yychar) is if
       this state is a consistent state with a default action.  Thus,
       detecting the absence of a lookahead is sufficient to determine
       that there is no unexpected or expected token to report.  In that
       case, just report a simple "syntax error".
     - Don't assume there isn't a lookahead just because this state is a
       consistent state with a default action.  There might have been a
       previous inconsistent state, consistent state with a non-default
       action, or user semantic action that manipulated yychar.
     - Of course, the expected token list depends on states to have
       correct lookahead information, and it depends on the parser not
       to perform extra reductions after fetching a lookahead from the
       scanner and before detecting a syntax error.  Thus, state merging
       (from LALR or IELR) and default reductions corrupt the expected
       token list.  However, the list is correct for canonical LR with
       one exception: it will still contain any token that will not be
       accepted due to an error action in a later state.
  */
  if (yytoken != YYEMPTY)
    {
      int yyn = yypact[+*yyssp];
      YYPTRDIFF_T yysize0 = yytnamerr (YY_NULLPTR, yytname[yytoken]);
      yysize = yysize0;
      yyarg[yycount++] = yytname[yytoken];
      if (!yypact_value_is_default (yyn))
        {
          /* Start YYX at -YYN if negative to avoid negative indexes in
             YYCHECK.  In other words, skip the first -YYN actions for
             this state because they are default actions.  */
          int yyxbegin = yyn < 0 ? -yyn : 0;
          /* Stay within bounds of both yycheck and yytname.  */
          int yychecklim = YYLAST - yyn + 1;
          int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
          int yyx;

          for (yyx = yyxbegin; yyx < yyxend; ++yyx)
            if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR
                && !yytable_value_is_error (yytable[yyx + yyn]))
              {
                if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
                  {
                    yycount = 1;
                    yysize = yysize0;
                    break;
                  }
                yyarg[yycount++] = yytname[yyx];
                {
                  YYPTRDIFF_T yysize1
                    = yysize + yytnamerr (YY_NULLPTR, yytname[yyx]);
                  if (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM)
                    yysize = yysize1;
                  else
                    return 2;
                }
              }
        }
    }

  switch (yycount)
    {
# define YYCASE_(N, S)                      \
      case N:                               \
        yyformat = S;                       \
      break
    default: /* Avoid compiler warnings. */
      YYCASE_(0, YY_("syntax error"));
      YYCASE_(1, YY_("syntax error, unexpected %s"));
      YYCASE_(2, YY_("syntax error, unexpected %s, expecting %s"));
      YYCASE_(3, YY_("syntax error, unexpected %s, expecting %s or %s"));
      YYCASE_(4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
      YYCASE_(5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
# undef YYCASE_
    }

  {
    /* Don't count the "%s"s in the final size, but reserve room for
       the terminator.  */
    YYPTRDIFF_T yysize1 = yysize + (yystrlen (yyformat) - 2 * yycount) + 1;
    if (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM)
      yysize = yysize1;
    else
      return 2;
  }

  if (*yymsg_alloc < yysize)
    {
      *yymsg_alloc = 2 * yysize;
      if (! (yysize <= *yymsg_alloc
             && *yymsg_alloc <= YYSTACK_ALLOC_MAXIMUM))
        *yymsg_alloc = YYSTACK_ALLOC_MAXIMUM;
      return 1;
    }

  /* Avoid sprintf, as that infringes on the user's name space.
     Don't have undefined behavior even if the translation
     produced a string with the wrong number of "%s"s.  */
  {
    char *yyp = *yymsg;
    int yyi = 0;
    while ((*yyp = *yyformat) != '\0')
      if (*yyp == '%' && yyformat[1] == 's' && yyi < yycount)
        {
          yyp += yytnamerr (yyp, yyarg[yyi++]);
          yyformat += 2;
        }
      else
        {
          ++yyp;
          ++yyformat;
        }
  }
  return 0;
}
#endif /* YYERROR_VERBOSE */

/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep)
{
  YYUSE (yyvaluep);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YYUSE (yytype);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}




/* The lookahead symbol.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;
/* Number of syntax errors so far.  */
int yynerrs;


/*----------.
| yyparse.  |
`----------*/

int
yyparse (void)
{
    yy_state_fast_t yystate;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       'yyss': related to states.
       'yyvs': related to semantic values.

       Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* The state stack.  */
    yy_state_t yyssa[YYINITDEPTH];
    yy_state_t *yyss;
    yy_state_t *yyssp;

    /* The semantic value stack.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs;
    YYSTYPE *yyvsp;

    YYPTRDIFF_T yystacksize;

  int yyn;
  int yyresult;
  /* Lookahead token as an internal (translated) token number.  */
  int yytoken = 0;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;

#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYPTRDIFF_T yymsg_alloc = sizeof yymsgbuf;
#endif

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  yyssp = yyss = yyssa;
  yyvsp = yyvs = yyvsa;
  yystacksize = YYINITDEPTH;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY; /* Cause a token to be read.  */
  goto yysetstate;


/*------------------------------------------------------------.
| yynewstate -- push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;


/*--------------------------------------------------------------------.
| yysetstate -- set current state (the top of the stack) to yystate.  |
`--------------------------------------------------------------------*/
yysetstate:
  YYDPRINTF ((stderr, "Entering state %d\n", yystate));
  YY_ASSERT (0 <= yystate && yystate < YYNSTATES);
  YY_IGNORE_USELESS_CAST_BEGIN
  *yyssp = YY_CAST (yy_state_t, yystate);
  YY_IGNORE_USELESS_CAST_END

  if (yyss + yystacksize - 1 <= yyssp)
#if !defined yyoverflow && !defined YYSTACK_RELOCATE
    goto yyexhaustedlab;
#else
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYPTRDIFF_T yysize = yyssp - yyss + 1;

# if defined yyoverflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        yy_state_t *yyss1 = yyss;
        YYSTYPE *yyvs1 = yyvs;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * YYSIZEOF (*yyssp),
                    &yyvs1, yysize * YYSIZEOF (*yyvsp),
                    &yystacksize);
        yyss = yyss1;
        yyvs = yyvs1;
      }
# else /* defined YYSTACK_RELOCATE */
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
        goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yy_state_t *yyss1 = yyss;
        union yyalloc *yyptr =
          YY_CAST (union yyalloc *,
                   YYSTACK_ALLOC (YY_CAST (YYSIZE_T, YYSTACK_BYTES (yystacksize))));
        if (! yyptr)
          goto yyexhaustedlab;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
# undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YY_IGNORE_USELESS_CAST_BEGIN
      YYDPRINTF ((stderr, "Stack size increased to %ld\n",
                  YY_CAST (long, yystacksize)));
      YY_IGNORE_USELESS_CAST_END

      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }
#endif /* !defined yyoverflow && !defined YYSTACK_RELOCATE */

  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;


/*-----------.
| yybackup.  |
`-----------*/
yybackup:
  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yypact_value_is_default (yyn))
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid lookahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = yylex ();
    }

  if (yychar <= YYEOF)
    {
      yychar = yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yytable_value_is_error (yyn))
        goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);
  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  /* Discard the shifted token.  */
  yychar = YYEMPTY;
  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     '$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
  case 2:
#line 813 "analizadorsintactico.y"
                       {addText("\tGT(-2);\n", &instructionsList); addText("END", &instructionsList);}
#line 2214 "analizadorsintactico.tab.c"
    break;

  case 14:
#line 824 "analizadorsintactico.y"
                           {writeReturn((yyvsp[0].reg));}
#line 2220 "analizadorsintactico.tab.c"
    break;

  case 15:
#line 825 "analizadorsintactico.y"
                                       {writeAssignment((yyvsp[-2].identifier), (yyvsp[0].reg));}
#line 2226 "analizadorsintactico.tab.c"
    break;

  case 16:
#line 826 "analizadorsintactico.y"
                              {struct Node* f = find((yyvsp[0].identifier)); checkArgumentCount(f); beginCallFunction(f); (yyval.reg) = callFunction(f);}
#line 2232 "analizadorsintactico.tab.c"
    break;

  case 17:
#line 827 "analizadorsintactico.y"
                                                           {struct Node* f = find((yyvsp[-2].identifier)); checkArgumentCount(f); beginCallFunction(f); (yyval.reg) = callFunction(f);}
#line 2238 "analizadorsintactico.tab.c"
    break;

  case 18:
#line 828 "analizadorsintactico.y"
                                                {checkArgumentCount((yyvsp[-2].symbol)); (yyval.reg) = callFunction((yyvsp[-2].symbol));}
#line 2244 "analizadorsintactico.tab.c"
    break;

  case 19:
#line 829 "analizadorsintactico.y"
                                                           {popContext(); printf("en llamada a función ¿Falta un paréntesis?\n"); checkArgumentCount((yyvsp[-2].symbol));}
#line 2250 "analizadorsintactico.tab.c"
    break;

  case 20:
#line 830 "analizadorsintactico.y"
                             {writePrint((yyvsp[0].reg));}
#line 2256 "analizadorsintactico.tab.c"
    break;

  case 21:
#line 831 "analizadorsintactico.y"
                                     {writePrintString((yyvsp[0].string));}
#line 2262 "analizadorsintactico.tab.c"
    break;

  case 22:
#line 832 "analizadorsintactico.y"
                                    {(yyval.symbol) = distinguishFunctionCall((yyvsp[-1].identifier));}
#line 2268 "analizadorsintactico.tab.c"
    break;

  case 23:
#line 833 "analizadorsintactico.y"
                                       {(yyval.identifier) = (yyvsp[-1].identifier); int label = getLabel(); 
						insert((yyvsp[-1].identifier), Function, label, inFunction); pushContext(); 
						lastFunctionArguments(currentContextVariables()); beginFunction(label);}
#line 2276 "analizadorsintactico.tab.c"
    break;

  case 24:
#line 836 "analizadorsintactico.y"
                                                                                     {(yyval.identifier) = (yyvsp[-3].symbol)->name; argumentCount = 0; 
					lastFunctionArguments(currentContextVariables());}
#line 2283 "analizadorsintactico.tab.c"
    break;

  case 25:
#line 838 "analizadorsintactico.y"
                                                     {endFunction(); popContext(); }
#line 2289 "analizadorsintactico.tab.c"
    break;

  case 26:
#line 839 "analizadorsintactico.y"
                                                          {printf("en definición de función ¿Falta un fin?\n");}
#line 2295 "analizadorsintactico.tab.c"
    break;

  case 27:
#line 840 "analizadorsintactico.y"
                                                          {(yyval.identifier) = (yyvsp[-3].identifier); argumentCount++; insert((yyvsp[0].identifier), getType((yyvsp[-1].string)), (argumentCount+2)*4, inFunction);}
#line 2301 "analizadorsintactico.tab.c"
    break;

  case 28:
#line 841 "analizadorsintactico.y"
                                                        {(yyval.identifier) = (yyvsp[0].identifier); argumentCount++; insert((yyvsp[0].identifier), getType((yyvsp[-1].string)), (argumentCount+2)*4, inFunction);}
#line 2307 "analizadorsintactico.tab.c"
    break;

  case 29:
#line 842 "analizadorsintactico.y"
                       {argumentCount++; addCallFunctionParameter((yyvsp[0].reg));}
#line 2313 "analizadorsintactico.tab.c"
    break;

  case 30:
#line 843 "analizadorsintactico.y"
                                                 {argumentCount++; addCallFunctionParameter((yyvsp[0].reg));}
#line 2319 "analizadorsintactico.tab.c"
    break;

  case 31:
#line 844 "analizadorsintactico.y"
                                  {pushContext(); writeConditionalStart((yyvsp[-1].reg));}
#line 2325 "analizadorsintactico.tab.c"
    break;

  case 32:
#line 845 "analizadorsintactico.y"
                      {writeElse();}
#line 2331 "analizadorsintactico.tab.c"
    break;

  case 33:
#line 846 "analizadorsintactico.y"
                                                                 {popContext(); writeConditionalEnd();}
#line 2337 "analizadorsintactico.tab.c"
    break;

  case 34:
#line 847 "analizadorsintactico.y"
                                                       {popContext(); writeConditionalEnd();}
#line 2343 "analizadorsintactico.tab.c"
    break;

  case 35:
#line 848 "analizadorsintactico.y"
                                                 {printf("en condición ¿Falta un fin?\n");}
#line 2349 "analizadorsintactico.tab.c"
    break;

  case 36:
#line 849 "analizadorsintactico.y"
                                                                         {printf("en condición ¿Falta un fin?\n");}
#line 2355 "analizadorsintactico.tab.c"
    break;

  case 37:
#line 850 "analizadorsintactico.y"
                                    {printf("en condición ¿Faltan los dos puntos?\n");}
#line 2361 "analizadorsintactico.tab.c"
    break;

  case 39:
#line 851 "analizadorsintactico.y"
                                                      {printf("en condición ¿Faltan los dos puntos?\n");}
#line 2367 "analizadorsintactico.tab.c"
    break;

  case 41:
#line 852 "analizadorsintactico.y"
                    {writeWhileLabel();}
#line 2373 "analizadorsintactico.tab.c"
    break;

  case 42:
#line 853 "analizadorsintactico.y"
                                        {pushContext(); writeWhileStart((yyvsp[-1].reg));}
#line 2379 "analizadorsintactico.tab.c"
    break;

  case 43:
#line 854 "analizadorsintactico.y"
                                   {popContext(); writeWhileEnd();}
#line 2385 "analizadorsintactico.tab.c"
    break;

  case 44:
#line 855 "analizadorsintactico.y"
                                      {popContext(); printf("en bucle ¿Falta un fin?\n");}
#line 2391 "analizadorsintactico.tab.c"
    break;

  case 45:
#line 856 "analizadorsintactico.y"
                                     {printf("en bucle ¿Faltan los dos puntos?\n");}
#line 2397 "analizadorsintactico.tab.c"
    break;

  case 47:
#line 857 "analizadorsintactico.y"
                                             {(yyval.reg) = writeOperation((yyvsp[-2].reg), (yyvsp[0].reg), "==");}
#line 2403 "analizadorsintactico.tab.c"
    break;

  case 48:
#line 858 "analizadorsintactico.y"
                                                                {(yyval.reg) = writeOperation((yyvsp[-2].reg), (yyvsp[0].reg), "!=");}
#line 2409 "analizadorsintactico.tab.c"
    break;

  case 49:
#line 859 "analizadorsintactico.y"
                                                              {(yyval.reg) = writeOperation((yyvsp[-2].reg), (yyvsp[0].reg), ">");}
#line 2415 "analizadorsintactico.tab.c"
    break;

  case 50:
#line 860 "analizadorsintactico.y"
                                                           {(yyval.reg) = writeOperation((yyvsp[-2].reg), (yyvsp[0].reg), "<");}
#line 2421 "analizadorsintactico.tab.c"
    break;

  case 51:
#line 861 "analizadorsintactico.y"
                                                                   {(yyval.reg) = writeOperation((yyvsp[-2].reg), (yyvsp[0].reg), ">=");}
#line 2427 "analizadorsintactico.tab.c"
    break;

  case 52:
#line 862 "analizadorsintactico.y"
                                                                {(yyval.reg) = writeOperation((yyvsp[-2].reg), (yyvsp[0].reg), "<=");}
#line 2433 "analizadorsintactico.tab.c"
    break;

  case 53:
#line 863 "analizadorsintactico.y"
                                            {(yyval.reg) = (yyvsp[0].reg);}
#line 2439 "analizadorsintactico.tab.c"
    break;

  case 54:
#line 864 "analizadorsintactico.y"
                                               {(yyval.reg) = writeOperation((yyvsp[-2].reg), (yyvsp[0].reg), "|");}
#line 2445 "analizadorsintactico.tab.c"
    break;

  case 55:
#line 865 "analizadorsintactico.y"
                                               {(yyval.reg) = (yyvsp[0].reg);}
#line 2451 "analizadorsintactico.tab.c"
    break;

  case 56:
#line 866 "analizadorsintactico.y"
                                                              {(yyval.reg) = writeOperation((yyvsp[-2].reg), (yyvsp[0].reg), "&");}
#line 2457 "analizadorsintactico.tab.c"
    break;

  case 57:
#line 867 "analizadorsintactico.y"
                                                           {(yyval.reg) = (yyvsp[0].reg);}
#line 2463 "analizadorsintactico.tab.c"
    break;

  case 58:
#line 868 "analizadorsintactico.y"
                                      {(yyval.reg) = (yyvsp[0].reg); writeBeforeRegister((yyvsp[0].reg), '~');}
#line 2469 "analizadorsintactico.tab.c"
    break;

  case 59:
#line 869 "analizadorsintactico.y"
                                                          {(yyval.reg) = (yyvsp[0].reg);}
#line 2475 "analizadorsintactico.tab.c"
    break;

  case 60:
#line 870 "analizadorsintactico.y"
                                  {(yyval.reg) = writeOperation((yyvsp[-2].reg), (yyvsp[0].reg), "+");}
#line 2481 "analizadorsintactico.tab.c"
    break;

  case 61:
#line 871 "analizadorsintactico.y"
                                              {(yyval.reg) = writeOperation((yyvsp[-2].reg), (yyvsp[0].reg), "-");}
#line 2487 "analizadorsintactico.tab.c"
    break;

  case 62:
#line 872 "analizadorsintactico.y"
                                {(yyval.reg) = (yyvsp[0].reg);}
#line 2493 "analizadorsintactico.tab.c"
    break;

  case 63:
#line 873 "analizadorsintactico.y"
                                          {(yyval.reg) = writeOperation((yyvsp[-2].reg), (yyvsp[0].reg), "*");}
#line 2499 "analizadorsintactico.tab.c"
    break;

  case 64:
#line 874 "analizadorsintactico.y"
                                                  {(yyval.reg) = writeOperation((yyvsp[-2].reg), (yyvsp[0].reg), "/");}
#line 2505 "analizadorsintactico.tab.c"
    break;

  case 65:
#line 875 "analizadorsintactico.y"
                                                     {(yyval.reg) = writeOperation((yyvsp[-2].reg), (yyvsp[0].reg), "%");}
#line 2511 "analizadorsintactico.tab.c"
    break;

  case 66:
#line 876 "analizadorsintactico.y"
                                      {(yyval.reg) = (yyvsp[0].reg);}
#line 2517 "analizadorsintactico.tab.c"
    break;

  case 67:
#line 877 "analizadorsintactico.y"
                            {(yyval.reg) = (yyvsp[0].reg); writeBeforeRegister((yyvsp[0].reg), '-');}
#line 2523 "analizadorsintactico.tab.c"
    break;

  case 68:
#line 878 "analizadorsintactico.y"
                                            {(yyval.reg) = (yyvsp[0].reg);}
#line 2529 "analizadorsintactico.tab.c"
    break;

  case 69:
#line 879 "analizadorsintactico.y"
                                        {(yyval.reg) = (yyvsp[0].reg);}
#line 2535 "analizadorsintactico.tab.c"
    break;

  case 70:
#line 880 "analizadorsintactico.y"
                                      {printf("en expresión ¿Falta un operando?\n");}
#line 2541 "analizadorsintactico.tab.c"
    break;

  case 71:
#line 881 "analizadorsintactico.y"
                      {(yyval.reg) = writeRegisterFloat((yyvsp[0].floating));}
#line 2547 "analizadorsintactico.tab.c"
    break;

  case 72:
#line 882 "analizadorsintactico.y"
                                {(yyval.reg) = writeRegisterInt((yyvsp[0].integer));}
#line 2553 "analizadorsintactico.tab.c"
    break;

  case 73:
#line 883 "analizadorsintactico.y"
                                  {(yyval.reg) = writeRegisterChar((yyvsp[0].character));}
#line 2559 "analizadorsintactico.tab.c"
    break;

  case 74:
#line 884 "analizadorsintactico.y"
                                   {checkExists((yyvsp[0].identifier)); (yyval.reg) = writeVariableAccess((yyvsp[0].identifier));}
#line 2565 "analizadorsintactico.tab.c"
    break;

  case 75:
#line 885 "analizadorsintactico.y"
                                     {(yyval.reg) = (yyvsp[0].reg);}
#line 2571 "analizadorsintactico.tab.c"
    break;

  case 76:
#line 886 "analizadorsintactico.y"
                                  {(yyval.reg) = writeRead((yyvsp[0].string));}
#line 2577 "analizadorsintactico.tab.c"
    break;

  case 77:
#line 887 "analizadorsintactico.y"
                                   {printf("en lectura desde usuario ¿Falta el tipo de dato?\n");}
#line 2583 "analizadorsintactico.tab.c"
    break;

  case 78:
#line 888 "analizadorsintactico.y"
                                  {(yyval.reg) = writeRegisterBool(-1);}
#line 2589 "analizadorsintactico.tab.c"
    break;

  case 79:
#line 889 "analizadorsintactico.y"
                              {(yyval.reg) = writeRegisterBool(0);}
#line 2595 "analizadorsintactico.tab.c"
    break;

  case 80:
#line 890 "analizadorsintactico.y"
                                           {(yyval.reg) = (yyvsp[-1].reg);}
#line 2601 "analizadorsintactico.tab.c"
    break;

  case 81:
#line 891 "analizadorsintactico.y"
                                             {printf("en expresión ¿Falta un paréntesis?\n");}
#line 2607 "analizadorsintactico.tab.c"
    break;


#line 2611 "analizadorsintactico.tab.c"

      default: break;
    }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */
  {
    const int yylhs = yyr1[yyn] - YYNTOKENS;
    const int yyi = yypgoto[yylhs] + *yyssp;
    yystate = (0 <= yyi && yyi <= YYLAST && yycheck[yyi] == *yyssp
               ? yytable[yyi]
               : yydefgoto[yylhs]);
  }

  goto yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYEMPTY : YYTRANSLATE (yychar);

  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (YY_("syntax error"));
#else
# define YYSYNTAX_ERROR yysyntax_error (&yymsg_alloc, &yymsg, \
                                        yyssp, yytoken)
      {
        char const *yymsgp = YY_("syntax error");
        int yysyntax_error_status;
        yysyntax_error_status = YYSYNTAX_ERROR;
        if (yysyntax_error_status == 0)
          yymsgp = yymsg;
        else if (yysyntax_error_status == 1)
          {
            if (yymsg != yymsgbuf)
              YYSTACK_FREE (yymsg);
            yymsg = YY_CAST (char *, YYSTACK_ALLOC (YY_CAST (YYSIZE_T, yymsg_alloc)));
            if (!yymsg)
              {
                yymsg = yymsgbuf;
                yymsg_alloc = sizeof yymsgbuf;
                yysyntax_error_status = 2;
              }
            else
              {
                yysyntax_error_status = YYSYNTAX_ERROR;
                yymsgp = yymsg;
              }
          }
        yyerror (yymsgp);
        if (yysyntax_error_status == 2)
          goto yyexhaustedlab;
      }
# undef YYSYNTAX_ERROR
#endif
    }



  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
         error, discard it.  */

      if (yychar <= YYEOF)
        {
          /* Return failure if at end of input.  */
          if (yychar == YYEOF)
            YYABORT;
        }
      else
        {
          yydestruct ("Error: discarding",
                      yytoken, &yylval);
          yychar = YYEMPTY;
        }
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:
  /* Pacify compilers when the user code never invokes YYERROR and the
     label yyerrorlab therefore never appears in user code.  */
  if (0)
    YYERROR;

  /* Do not reclaim the symbols of the rule whose action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;      /* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
        {
          yyn += YYTERROR;
          if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
            {
              yyn = yytable[yyn];
              if (0 < yyn)
                break;
            }
        }

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
        YYABORT;


      yydestruct ("Error: popping",
                  yystos[yystate], yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", yystos[yyn], yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;


/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;


#if !defined yyoverflow || YYERROR_VERBOSE
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif


/*-----------------------------------------------------.
| yyreturn -- parsing is finished, return the result.  |
`-----------------------------------------------------*/
yyreturn:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  yystos[+*yyssp], yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
#if YYERROR_VERBOSE
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
#endif
  return yyresult;
}
#line 892 "analizadorsintactico.y"


void openFile(char* inputFileName) {
	char* extensionPointer = inputFileName;
	int size = 0;
	while (*extensionPointer != '\0') {
		extensionPointer++;
		size++;
	}
	extensionPointer -= 5;
	size -= 5;
	if (size > 0 && strcmp(extensionPointer, ".luma") == 0) {
		*extensionPointer = '\0';
	}
}

int main(int argc, char** argv) {
	if (argc>1) {
		char* filename = argv[1];
		yyin=fopen(filename,"r");
		openFile(filename);
		functionPosition = staticPosition + 1;
		insertRegisters();
		addText("#include \"Q.h\"\n", &instructionsList);
		addText("BEGIN\n", &instructionsList);
		addText("L 0:\n", &instructionsList);
		yyparse();
		writeText(filename, &instructionsList);
	} else {
		printf("Falta un argumento (el fichero que se va a compilar)\n");
	}
}

void yyerror(char* mens) {
	printf("Error en linea %i: %s\n",line,mens);
}
