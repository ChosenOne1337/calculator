#include "expr_string.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define BUF_SIZE 2048
static char buf[BUF_SIZE];

String makeStrCpy(String str) {
    //allocates memory for a copy of contents of the buffer
    String str_cpy = (String)malloc((strlen(str) + 1) * sizeof(char));
    if (str_cpy != NULL) {
        strcpy(str_cpy, str);
    }
    return str_cpy;
}

String readString(void) {
    //reads a string from stdin
    //allocates memory for a string
    gets(buf);
    return makeStrCpy(buf);
}

String removeSpaces(String str) {
    //removes all spaces in the string;
    //allocates memory for a new one
    buf[0] = '\0'; //empty the buffer
    String str_copy = makeStrCpy(str);
    String word = strtok(str_copy, " \t");
    while (word != NULL) {
        strcat(buf, word);
        word = strtok(NULL, " \t");
    }
    destroyString(str_copy);
    return makeStrCpy(buf);
}

int isExpr(String expr) {
    //checks whether the expression contains invalid characters or not
    //(which are not in the charset below)
    static char charset[] = "0123456789+-*/(). \t";
    while (*expr != '\0') {
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
