#define __STDC_WANT_LIB_EXT2__ 1
#include "expr_string.h"
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define BUF_SIZE 2048
static char buf[BUF_SIZE];

char *read_string(FILE *pFile) {
    if (fgets(buf, BUF_SIZE, pFile) == NULL) {
        return NULL;
    }
    return strdup(buf);
}

void remove_spaces(char *str) {
    //removes all spaces in the string;
    if (str == NULL) {
        return;
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
}

int has_letters(const char *expr) {
    while (*expr) {
        if (isdigit(*expr++)) {
            return 1;
        }
    }
    return 0;
}

int is_empty(const char *expr) {
    return expr == NULL || *expr == '\0';
}

void destroy_string(char *str) {
    if (str != NULL) {
        free(str);
    }
}
