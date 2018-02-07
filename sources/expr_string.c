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

char *removeSpaces(char *str) {
    //removes all spaces in the string;
    if (str == NULL) {
        return NULL;
    }
    static char charset[] = " \t\n";
    int len = 0;
    char *str_glued = str;
    for (; *str; ++str) {
        if (strchr(charset, *str) == NULL) {
            str_glued[len++] = *str;
        }
    }
    str_glued[len] = '\0';
    return str_glued;
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
