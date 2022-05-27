#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct ListNode
{
    char *text;
    struct ListNode *next;
};

int staticZoneWritten = 0;
int staticPosition = 1;
int functionPosition, beginningFunction;
int inFunction = 0;

char *copyString(char *str)
{
  char *tmp = malloc(strlen(str) + 1);
  if (tmp)
    strcpy(tmp, str);
  return tmp;
}

void addTextAtPos(char* text, int pos, struct ListNode** head) {
    struct ListNode *newNode = (struct ListNode *)malloc(sizeof(struct ListNode));
    newNode->text = copyString(text);

    struct ListNode *current = *head;

    for(int i = 1; i < pos; i++) {
        if(current->next != NULL) {
            current = current->next;
        }
    }
    newNode->next = current->next;
    current->next = newNode;
}

void addText(char *text, struct ListNode **head) {
    if (inFunction) {
        addTextAtPos(text, functionPosition, head);
        functionPosition++;
        return;
    }
    struct ListNode *newNode = (struct ListNode *)malloc(sizeof(struct ListNode));
    newNode->text = copyString(text);
    newNode->next = NULL;
    if (*head == NULL) {
        *head = newNode;
        return;
    }
    struct ListNode *current = *head;
    while (current->next != NULL) {
        current = current->next;
    }
    current->next = newNode;
}

int getTextSize(struct ListNode** head) {
    struct ListNode *current = *head;
    int size = 0;
    while (current != NULL) {
        current = current->next;
        size++;
    }
    return size;
}

void writeText(char *filename, struct ListNode **head) {
    filename = strcat(filename, ".q.c"); 
    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        printf("Error al escribir archivo %s\n", filename);
        exit(1);
    }

    struct ListNode *current = *head;
    while (current != NULL)
    {
        fprintf(file, "%s", current->text);
        current = current->next;
    }
    fclose(file);
}

void showFile(struct ListNode **head) {
    struct ListNode *current = *head;
    int index = 0;
    while (current != NULL)
    {
        printf("Block %i: %s", index, current->text);
        current = current->next;
        index++;
    }
    printf("\n");
}