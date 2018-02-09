#include "stack.h"
#include <stdlib.h>

        /// List ///

List *tail(List *pHead) {
    if (pHead == NULL) {
        return NULL;
    }
    while (pHead->pNext != NULL) {
        pHead = pHead->pNext;
    }
    return pHead;
}

List *prepend(List *pHead, Token data) {
    //if succeeds, returns pointer to the new head,
    //else - pointer to the old one
    List *pNew = (List*)malloc(sizeof(List));
    if (pNew == NULL) {
        return pHead;
    }
    pNew->data = data;
    pNew->pNext = pHead;
    return pNew;
}

List *append(List *pHead, Token data) {
    //if pHead == NULL, returns pointer to the new head
    //else - pointer to the old one
    List *pNew = (List*)malloc(sizeof(List));
    if (pNew == NULL) {
        return pHead;
    }
    pNew->data = data;
    pNew->pNext = NULL;
    if (pHead != NULL) {
        tail(pHead)->pNext = pNew;
        return pHead;
    }
    return pNew;
}

List *insert_after(List *pPrev, Token data) {
    //returns pointer to the added item
    //or pPrev if nothing has been added
    //if pPrev == NULL, function creates an element anyway
    List *pNew = (List*)malloc(sizeof(List));
    if (pNew == NULL) {
        return pPrev;
    }
    pNew->data = data;
    if (pPrev != NULL) {
        pNew->pNext = pPrev->pNext;
        pPrev->pNext = pNew;
    }
    else {
        pNew->pNext = NULL;
    }
    return pNew;
}

List *erase_after(List *pPrev) {
    //returns pointer to the item that follows the removed element;
    if (pPrev == NULL || pPrev->pNext == NULL) {
        return NULL;
    }
    List *pDel = pPrev->pNext;
    pPrev->pNext = pDel->pNext;
    free(pDel);
    return pPrev->pNext;
}

List *erase_head(List *pHead) {
    if (pHead != NULL) {
        List *pDel = pHead;
        pHead = pHead->pNext;
        free(pDel);
    }
    return pHead;
}

List *erase_tail(List *pHead) {
    List *pTail= tail(pHead);
    if (pTail != NULL) {
        free(pTail);
    }
    return pHead;
}

void destroy_list(List *pHead) {
    while (pHead) {
        pHead = erase_head(pHead);
    }
}

        /// Stack ///

Stack *create_stack(void) {
    Stack *pStack = (Stack*)malloc(sizeof(Stack));
    if (pStack != NULL) {
        pStack->pTop = NULL;
        pStack->size = 0;
    }
    return pStack;
}

void destroy_stack(Stack *pStack) {
    if (pStack != NULL) {
        destroy_list(pStack->pTop);
        free(pStack);
    }
}

int empty(Stack *pStack) {
    return pStack->pTop == NULL;
}

void push(Stack *pStack, Token data) {
    if (pStack != NULL) {
        pStack->pTop = prepend(pStack->pTop, data);
        ++pStack->size;
    }
}

Token pop(Stack *pStack) {
    Token data = pStack->pTop->data;
    pStack->pTop = erase_head(pStack->pTop);
    --pStack->size;
    return data;
}

Token top(Stack *pStack) {
    return pStack->pTop->data;
}
