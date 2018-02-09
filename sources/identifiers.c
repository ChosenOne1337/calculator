#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdio.h>
#include "identifiers.h"
#include "expr_string.h"
#include "error.h"

static char **valueList = NULL; //array of variables' values
static char **variableList = NULL; //array of variables' names
static int VAR_LIST_LEN = 0;
static int VAR_NUM = 0;

int is_var_declaration(char *expr) {
    //an expression without white spaces is expected
    //identificator must start from a letter
    //and can contain only letters and digits
    if (strchr(expr, '=') == NULL) {
        //expression doesn't contain '='
        return 0;
    }
    if (!isalpha(*expr)) {
        set_error(InvalidIdentifierError);
        return 1;
    }
    char *expr_cpy = strdup(expr);
    char *token = strtok(expr_cpy, "=");
    while (*token) {
        if (!isalnum(*token++)) {
            set_error(InvalidIdentifierError);
            return 1;
        }
    }
    token = strtok(NULL, "");
    if (token == NULL) {
        set_error(InvalidValueError);
        return 1;
    }
    strtod(token, &token);
    if (*token) {
        set_error(InvalidValueError);
    }
    destroy_string(expr_cpy);
    return 1;
}

int get_var_index(char *varName) {
    //returns index of the variable 'varName' in the variableList
    //or -1 if it isn't in the list
    int i;
    for (i = 0; i < VAR_NUM; ++i) {
        if (!strcmp(varName, variableList[i])) {
            return i;
        }
    }
    return -1;
}

char *get_var_value(char *varName) {
    //returns NULL if variable isn't in the list
    int ix = get_var_index(varName);
    if (ix < 0) {
        return NULL;
    }
    return valueList[ix];
}

char *extract_var(char *expr) {
    char *ptr = expr;
    while (isalnum(*ptr)) {
        ++ptr;
    }
    int len = ptr - expr;
    char *varName = (char*)malloc(len + 1);
    memcpy(varName, expr, len);
    varName[len] = '\0';
    return varName;
}

void add_var(char *expr) {
    //valid variable declaration is expected
    //expr will be changed by strtok(..) function
    if (VAR_LIST_LEN == 0) {
        variableList = (char**)malloc(sizeof(char*));
        valueList = (char**)malloc(sizeof(char*));
        VAR_LIST_LEN = 1;
    }
    if (VAR_LIST_LEN == VAR_NUM) {
        VAR_LIST_LEN *= 2;
        variableList = (char**)realloc(variableList, VAR_LIST_LEN * sizeof(char*));
        valueList = (char**)realloc(valueList, VAR_LIST_LEN * sizeof(char*));
    }
    char *token = strtok(expr, "=");
    int ix = get_var_index(token);
    if (ix == -1) {
        variableList[VAR_NUM] = strdup(token);
        valueList[VAR_NUM] = strdup(strtok(NULL, ""));
        ++VAR_NUM;
    }
    else {
        free(valueList[ix]);
        valueList[ix] = strdup(strtok(NULL, ""));
    }
}

void print_var_list() {
    int i;
    for (i = 0; i < VAR_NUM; ++i) {
        printf("%s = %s\n", variableList[i], valueList[i]);
    }
}

void destroy_var_list() {
    if (VAR_LIST_LEN) {
        int i;
        for (i = 0; i < VAR_NUM; ++i) {
            free(variableList[i]);
            free(valueList[i]);
        }
        free(variableList);
        free(valueList);
    }
}
