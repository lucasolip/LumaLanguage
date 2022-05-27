%{
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



%}

%union { char* identifier; struct Variable reg; struct Node* symbol; 
			float floating; char character; int integer; char* string; }
%token WRITE READ RETURN WHILE IF ELSE END MODULO NOT VERDADERO FALSO 
		NEWINSTRUCTION GREATER LESS EQUALS GREATEREQUAL LESSEQUAL DIFFERENT
%token <identifier> IDENTIFIER
%token <floating> FLOAT
%token <character> CHARACTER
%token <integer> INTEGER
%token <string> STRING
%token <string> TYPE

%type <identifier> definingParameter
%type <identifier> functionHeader
%type <symbol> functionHeaderOpen
%type <reg> functionCall
%type <reg> bareFunctionCall
%type <reg> operand
%type <reg> signedOperand
%type <reg> expression
%type <reg> proposition
%type <reg> andProposition
%type <reg> negatedProposition
%type <reg> operation
%type <reg> product

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
program : instructions {addText("\tGT(-2);\n", &instructionsList); addText("END", &instructionsList);};
instructions : instruction | instructions NEWINSTRUCTION instruction;
instruction : 	assignment | 
		control | 
		loop | 
		functionDefinition | 
		functionCall | 
		writeCall |
		bareFunctionCall |
		/* Instrucción vacía */ | 
		return ;
return : RETURN expression {writeReturn($2);};
assignment : IDENTIFIER '=' expression {writeAssignment($1, $3);};
bareFunctionCall : IDENTIFIER {struct Node* f = find($1); checkArgumentCount(f); beginCallFunction(f); $$ = callFunction(f);} | 
					IDENTIFIER '(' ')' {struct Node* f = find($1); checkArgumentCount(f); beginCallFunction(f); $$ = callFunction(f);};
functionCall : functionHeaderOpen parameter ')' {checkArgumentCount($1); $$ = callFunction($1);} | 
			functionHeaderOpen parameter error {popContext(); printf("en llamada a función ¿Falta un paréntesis?\n"); checkArgumentCount($1);};
writeCall : WRITE expression {writePrint($2);} | 
			WRITE STRING {writePrintString($2);};
functionHeaderOpen : IDENTIFIER '(' {$$ = distinguishFunctionCall($1);};
functionHeader : 	IDENTIFIER ':' {$$ = $1; int label = getLabel(); 
						insert($1, Function, label, inFunction); pushContext(); 
						lastFunctionArguments(currentContextVariables()); beginFunction(label);} | 
					functionHeaderOpen definingParameter ')' ':' {$$ = $1->name; argumentCount = 0; 
					lastFunctionArguments(currentContextVariables());};
functionDefinition : functionHeader instructions END {endFunction(); popContext(); } | 
			functionHeader instructions error {printf("en definición de función ¿Falta un fin?\n");};
definingParameter : definingParameter ',' TYPE IDENTIFIER {$$ = $1; argumentCount++; insert($4, getType($3), (argumentCount+2)*4, inFunction);} | 
					TYPE IDENTIFIER {$$ = $2; argumentCount++; insert($2, getType($1), (argumentCount+2)*4, inFunction);};
parameter : expression {argumentCount++; addCallFunctionParameter($1);} | 
			parameter ',' expression {argumentCount++; addCallFunctionParameter($3);};
controlHeader : IF expression ':' {pushContext(); writeConditionalStart($2);};
elseHeader : ELSE ':' {writeElse();};
control : controlHeader instructions elseHeader instructions END {popContext(); writeConditionalEnd();} | 
			controlHeader instructions END {popContext(); writeConditionalEnd();} | 
		controlHeader instructions error {printf("en condición ¿Falta un fin?\n");} |
		controlHeader instructions elseHeader instructions error {printf("en condición ¿Falta un fin?\n");} |
		IF expression error {printf("en condición ¿Faltan los dos puntos?\n");} instructions END | 
		controlHeader instructions ELSE error {printf("en condición ¿Faltan los dos puntos?\n");} instructions END;
loopKeyword : WHILE {writeWhileLabel();};
loopHeader : loopKeyword expression ':' {pushContext(); writeWhileStart($2);};
loop : loopHeader instructions END {popContext(); writeWhileEnd();} | 
	loopHeader instructions error {popContext(); printf("en bucle ¿Falta un fin?\n");} |
	loopKeyword expression error {printf("en bucle ¿Faltan los dos puntos?\n");} instructions END ;
expression : 	expression EQUALS expression {$$ = writeOperation($1, $3, "==");} | 
				expression DIFFERENT expression {$$ = writeOperation($1, $3, "!=");} | 
				expression GREATER expression {$$ = writeOperation($1, $3, ">");} | 
				expression LESS expression {$$ = writeOperation($1, $3, "<");} | 
				expression GREATEREQUAL expression {$$ = writeOperation($1, $3, ">=");} | 
				expression LESSEQUAL expression {$$ = writeOperation($1, $3, "<=");} | 
				proposition {$$ = $1;};
proposition : 	proposition 'o' andProposition {$$ = writeOperation($1, $3, "|");} | 
				andProposition {$$ = $1;};
andProposition : 	andProposition 'y' negatedProposition {$$ = writeOperation($1, $3, "&");} | 
					negatedProposition {$$ = $1;};
negatedProposition : 	NOT operation {$$ = $2; writeBeforeRegister($2, '~');} | 
						operation {$$ = $1;};
operation : operation '+' product {$$ = writeOperation($1, $3, "+");} | 
			operation '-' product {$$ = writeOperation($1, $3, "-");} | 
			product {$$ = $1;};
product : 	product '*' signedOperand {$$ = writeOperation($1, $3, "*");} | 
			product '/' signedOperand {$$ = writeOperation($1, $3, "/");} | 
			product MODULO signedOperand {$$ = writeOperation($1, $3, "%");} | 
			signedOperand {$$ = $1;};
signedOperand : '-' operand {$$ = $2; writeBeforeRegister($2, '-');} | 
				'+' operand {$$ = $2;} | 
				operand {$$ = $1;} | 
				error {printf("en expresión ¿Falta un operando?\n");};
operand : 	FLOAT {$$ = writeRegisterFloat($1);} | 
			INTEGER {$$ = writeRegisterInt($1);} |
			CHARACTER {$$ = writeRegisterChar($1);} | 
			IDENTIFIER {checkExists($1); $$ = writeVariableAccess($1);} |
			functionCall {$$ = $1;} | 
			READ TYPE {$$ = writeRead($2);} | 
			READ error {printf("en lectura desde usuario ¿Falta el tipo de dato?\n");} |
			VERDADERO {$$ = writeRegisterBool(-1);} | 
			FALSO {$$ = writeRegisterBool(0);} | 
			'(' expression ')' {$$ = $2;} | 
			'(' expression error {printf("en expresión ¿Falta un paréntesis?\n");};
%%

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
