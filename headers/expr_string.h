#ifndef EXPR_STRING_H
#define EXPR_STRING_H

#include <stdio.h>

char *read_string(FILE *pFile);
void remove_spaces(char *str);
void destroy_string(char *str);
int has_letters(const char *expr);
int is_empty(const char *expr);

#endif // EXPR_STRING_H
