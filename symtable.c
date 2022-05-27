#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "types.c"

struct Node {
    char* name;
    enum Type type;
    int address;
    int numArguments;
    struct Node* next;
    int fromFunction;
    enum Type returnType;
};

struct Node* top = NULL;

int contains(char* name);

struct Node* insert(char* name, enum Type type, int address, int inFunction) {
    if (contains(name)) return NULL;
    struct Node* newNode = (struct Node*)malloc(sizeof(struct Node));
    newNode->name = name;
    newNode->type = type;
    newNode->address = address;
    newNode->numArguments = -1;
    newNode->fromFunction = inFunction;
    newNode->returnType = Void;
    newNode->next = top;
    top = newNode;
    return top;
}

int contains(char* name) {
    struct Node* current = top;
    while (current != NULL) {
        if (strcmp(name, current->name) == 0 && name[0] != '-') return 1;
        current = current->next;
    }
    return 0;
}

struct Node* find(char* name) {
    struct Node* current = top;
    while (current != NULL) {
        if (strcmp(name, current->name) == 0 && name[0] != '-') return current;
        current = current->next;
    }
    return NULL;
}

void pop(int numNodes) {
    int n = 0;
    struct Node* current = NULL;
    while (top != NULL && n < numNodes) {
        printf("Eliminado el nodo %s\n", top->name);
        current = top;
        top = top->next;
        free(current);
        n++;
    }
}

int lastFunctionArguments(int n) {
    struct Node* current = top;
    while (current != NULL) {
        if (current->type == Function) {
            current->numArguments = n;
            return 1;
        }
        current = current->next;
    }
    printf("No se encontró ninguna función\n");
    return 0;
}

int setFunctionType(enum Type type) {
    struct Node* current = top;
    while (current != NULL) {
        if (current->type == Function) {
            current->returnType = type;
            return 1;
        }
        current = current->next;
    }
    printf("No se encontró ninguna función\n");
    return 0;
}

void pushContext() {
    insert("-", ContextSeparator, 0, 0);
}

void popContext() {
    struct Node* current = NULL;
    while (top != NULL && top->type != ContextSeparator) {
        current = top;
        top = top->next;
        free(current);
    }
    if (top != NULL) {
        current = top;
        top = top->next;
        free(current);
    }
}

int currentContextVariables() {
    struct Node* current = top;
    int nVariables = 0;
    while (current != NULL && current->type != ContextSeparator) {
        nVariables++;
        current = current->next;
    }
    return nVariables;
}

int getNumArguments(char* functionName) {
    struct Node* current = top;
    while (current != NULL) {
        if (current->type == Function && strcmp(functionName, current->name) == 0) 
            return current->numArguments;
        current = current->next;
    }
    return -1;
}

void printStack() {
    struct Node* current = top;
    while (current != NULL) {
        printf("%s, %d, %d, %d\n", current->name, current->type, current->address, current->numArguments);
        current = current->next;
    }
}