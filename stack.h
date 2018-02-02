#ifndef STACK_H
#define STACK_H

#include "token_parser.h"

        /// List ///

typedef struct List {
    Token data;
    struct List *pNext;
} List;

List *prepend(List *pHead, Token data);
List *append(List *pHead, Token data);
List *insert_after(List *pPrev, Token data);
List *erase_after(List *pPrev);
List *erase_head(List *pHead);
void destroyList(List *pHead);

        /// Stack ///

typedef struct Stack {
    List *pTop;
    int size;
} Stack;

Stack *createStack(void);
void destroyStack(Stack *pStack);
int empty(Stack *pStack);
void push(Stack *pStack, Token data);
Token pop(Stack *pStack);
Token top(Stack *pStack);

#endif // STACK_H
