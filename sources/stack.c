#include "stack.h"
#include <stdlib.h>

        /// List ///

List *prepend(List *pHead, Token data) {
    //if succeeds, returns pointer to the new head,
    //else - pointer to the old one
    List *pNew = (List*)malloc(sizeof(List));
    if (pNew != NULL) {
        pNew->data = data;
        pNew->pNext = pHead;
        return pNew;
    }
    return pHead;
}

List *append(List *pHead, Token data) {
    //if pHead == NULL, returns pointer to the new head
    //else - pointer to the old one
    List *pNew = (List*)malloc(sizeof(List));
    if (pNew != NULL) {
        pNew->data = data;
        pNew->pNext = NULL;
        if (pHead != NULL) {
            List *pCur = pHead;
            while (pCur->pNext != NULL) {
                pCur = pCur->pNext;
            }
            pCur->pNext = pNew;
        }
        else return pNew;
    }
    return pHead;
}

List *insert_after(List *pPrev, Token data) {
    //returns pointer to the added item
    //or pPrev if nothing has been added
    //if pPrev == NULL, function creates an element anyway
    List *pNew = (List*)malloc(sizeof(List));
    if (pNew != NULL) {
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
    return pPrev;
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
    List *pCur = pHead, *pPrev = NULL;
    if (pHead != NULL) {
        while (pCur->pNext != NULL) {
            pPrev = pCur;
            pCur = pCur->pNext;
        }
        if (pPrev != NULL) {
            pPrev->pNext = NULL;
        }
        free(pCur);
    }
    return pPrev;
}

void destroyList(List *pHead) {
    List *pCur;
    while (pHead != NULL) {
        pCur = pHead;
        pHead = pHead->pNext;
        free(pCur);
    }
}

        /// Stack ///

Stack *createStack(void) {
    Stack *pStack = (Stack*)malloc(sizeof(Stack));
    if (pStack != NULL) {
        pStack->pTop = NULL;
        pStack->size = 0;
    }
    return pStack;
}

void destroyStack(Stack *pStack) {
    if (pStack != NULL) {
        destroyList(pStack->pTop);
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
