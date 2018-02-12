#ifndef STACK_H
#define STACK_H

#include "token_parser.h"

        /// List ///

typedef struct List {
    Token token;
    struct List *pNext;
} List;

List *tail(List *pHead);
List *prepend(List *pHead, Token data);
List *append(List *pHead, Token data);
List *insert_after(List *pPrev, Token data);
List *erase_after(List *pPrev);
List *erase_head(List *pHead);
List *erase_tail(List *pHead);
void destroy_list(List *pHead);

        /// Stack ///

typedef struct Stack {
    List *pTop;
    int size;
} Stack;

Stack *create_stack(void);
void destroy_stack(Stack *pStack);
int empty(Stack *pStack);
void push(Stack *pStack, Token data);
Token pop(Stack *pStack);
Token top(Stack *pStack);

#endif // STACK_H
