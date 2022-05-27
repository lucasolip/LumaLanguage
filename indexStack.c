#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct IndexNode {
    int data;
    struct IndexNode* next;
};

void insertIndex(int data, struct IndexNode** top) {
    struct IndexNode *newNode = (struct IndexNode *)malloc(sizeof(struct IndexNode));
    newNode->data = data;
    newNode->next = *top;
    *top = newNode;
}

int getIndex(struct IndexNode** top) {
    return (*top)->data;
}

int popIndex(struct IndexNode** top) {
    int data = (*top)->data;
    //free(*top);
    *top = (*top)->next;
    return data;
}

void printIndexStack(struct IndexNode** top) {
    struct IndexNode* current = *top;
    while (current != NULL) {
        printf("%d\n", current->data);
        current = current->next;
    }
}