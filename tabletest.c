#include <stdio.h>
#include "symtable.c"

void main() {
    
    insert("Pepi", Integer);
    insert("José", Function);
    printStack();
    pushContext();
    insert("Manuela", Float);
    lastFunctionArguments(currentContextVariables());
    printStack();
    pushContext();
    insert("Pepi", Integer);
    insert("Juana", Float);
    printStack();
    popContext();
    printStack();
    popContext();

    printStack();
}