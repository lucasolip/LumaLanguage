#include <stdio.h>
#include "textList.c"
#include "indexStack.c"

void main() {
    struct ListNode* instructionsList = NULL;
    addText("Pepi\n", &instructionsList);
    addText("Manoli\n", &instructionsList);
    addText("Ruchi\n", &instructionsList);
    addText("Cuchi\n", &instructionsList);
    int position = getTextSize(&instructionsList);
    addText("Majuchi\n", &instructionsList);
    addTextAtPos("Nuevo texto\n", position, &instructionsList);
    showFile(&instructionsList);

    struct IndexNode* indexStack = NULL;
    insertIndex(5, &indexStack);
    insertIndex(7, &indexStack);
    //printIndexStack(&indexStack);
    popIndex(&indexStack);
    popIndex(&indexStack);
    //printIndexStack(&indexStack);
}