#define __STDC_WANT_LIB_EXT2__ 1
#include "expr_string.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define BUF_SIZE 2048
static char buf[BUF_SIZE];

String readString(void) {
    //reads a string from stdin
    //allocates memory for a string
    fgets(buf, BUF_SIZE, stdin);
    return strdup(buf);
}

String removeSpaces(String str) {
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

int isExpr(String expr) {
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

void destroyString(String str) {
    if (str != NULL) {
        free(str);
    }
}
