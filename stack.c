#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum Type {
    Function,
    Integer,
    Float,
    Character,
    String,
    Bool
};

struct Node {
    char* name;
    enum Type type;
    struct Node* next;
};

struct Node* top = NULL;

void insert(char* name) {
    struct Node* newNode = (struct Node*)malloc(sizeof(struct Node));
    newNode->name = name;
    newNode->next = top;
    top = newNode;
}

int contains(char* name) {
    struct Node* current = top;
    while (current != NULL) {
        if (strcmp(name, current->name) == 0) return 1;
        current = current->next;
    }
    return 0;
}

void delete(int numNodes) {
    int n = 0;
    struct Node* current = top;
    while (current != NULL && n < numNodes) {
        printf("Eliminado el nodo %s\n", top->name);
        top = top->next;
        n++;
    }
}

void printStack() {
    struct Node* current = top;
    while (current != NULL) {
        printf("%s\n", current->name);
        current = current->next;
    }
}

void main() {
    insert("Manolo");
    insert("José");
    insert("Maripili");

    printStack();
}