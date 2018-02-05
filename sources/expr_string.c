#define __STDC_WANT_LIB_EXT2__ 1
#include "expr_string.h"
#include <string.h>
#include <stdlib.h>

#define BUF_SIZE 2048
static char buf[BUF_SIZE];

char *readString(FILE *pFile) {
    if (fgets(buf, BUF_SIZE, pFile) == NULL) {
        return NULL;
    }
    return strdup(buf);
}

char *removeSpaces(const char *str) {
    //removes all spaces in the string;
    //allocates memory for a new one
    static char charset[] = " \t\n";
    int len = 0;
    for (; *str; ++str) {
        if (strchr(charset, *str) == NULL) {
            buf[len++] = *str;
        }
    }
    buf[len] = '\0';
    return strdup(buf);
}

int isExpr(const char *expr) {
    //checks whether the expression contains invalid characters or not
    //(which are not in the charset below)
    static char charset[] = "0123456789+-*/().";
    while (*expr) {
        if (strchr(charset, *expr++) == NULL) {
            return 0;
        }
    }
    return 1;
}

void destroyString(char *str) {
    if (str != NULL) {
        free(str);
    }
}
