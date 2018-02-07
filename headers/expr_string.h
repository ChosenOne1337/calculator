#ifndef EXPR_STRING_H
#define EXPR_STRING_H

#include <stdio.h>

char *readString(FILE *pFile);
char *removeSpaces(char *str);
void destroyString(char *str);
int isExpr(const char *expr);

#endif // EXPR_STRING_H
