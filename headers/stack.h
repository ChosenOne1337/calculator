#ifndef STACK_H
#define STACK_H

#include "token_parser.h"

//typedef enum ItemType {
//    CHAR,
//    INT,
//    DOUBLE,
//    TOKEN
//} ItemType;

//typedef struct Item {
//    ItemType tag;
//    typedef union ItemUnion {
//        char char_val;
//        int int_val;
//        double double_val;
//        Token token_val;
//    } ItemUnion;
//} Item;

        /// List ///

typedef struct List {
    Token data;
    struct List *pNext;
} List;

List *tail(List *pHead);
List *prepend(List *pHead, Token data);
List *append(List *pHead, Token data);
List *insert_after(List *pPrev, Token data);
List *erase_after(List *pPrev);
List *erase_head(List *pHead);
List *erase_tail(List *pHead);
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
