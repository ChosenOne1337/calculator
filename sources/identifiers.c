#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdio.h>
#include "identifiers.h"
#include "error.h"

static char **valueList = NULL; //array of variables' values
static char **variableList = NULL; //array of variables' names
static int VAR_LIST_LEN = 0;
static int VAR_NUM = 0;

int is_var_declaration(char *expr) {
    //an expression without white spaces is expected:
    //id_name=id_value
    //identificator must start from a letter
    //and can contain only letters and digits
    if (strchr(expr, '=') == NULL) {
        return 0;
    }
    if (!isalpha(*expr++)) {
        set_error(InvalidIdentifierError);
        return 1;
    }
    while (*expr != '=') {
        if (!isalnum(*expr++)) {
            set_error(InvalidIdentifierError);
            return 1;
        }
    }
    ++expr;
    if (!*expr) {
        set_error(InvalidValueError);
        return 1;
    }
    strtod(expr, &expr);
    if (*expr) {
        set_error(InvalidValueError);
    }
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

void init_var_list(void) {
    variableList = (char**)malloc(sizeof(char*));
    valueList = (char**)malloc(sizeof(char*));
    VAR_LIST_LEN = 1;
}

void expand_var_list(void) {
    VAR_LIST_LEN *= 2;
    variableList = (char**)realloc(variableList, VAR_LIST_LEN * sizeof(char*));
    valueList = (char**)realloc(valueList, VAR_LIST_LEN * sizeof(char*));
}

void print_var_list(void) {
    int i;
    for (i = 0; i < VAR_NUM; ++i) {
        printf("%s = %s\n", variableList[i], valueList[i]);
    }
}

void destroy_var_list(void) {
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

void extract_var(char *expr, char **varName, char **varValue) {
    //get variable name/value from the variable declaration
    char *curPtr = expr;
    for (; *curPtr != '='; ++curPtr);
    *curPtr = '\0';
    *varName = strdup(expr);
    *curPtr++ = '=';
    *varValue = strdup(curPtr);
}

void add_var(char *expr) {
    //valid variable declaration is expected
    if (VAR_LIST_LEN == 0) {
        init_var_list();
    }
    if (VAR_LIST_LEN == VAR_NUM) {
        expand_var_list();
    }
    char *varName = NULL, *varValue = NULL;
    extract_var(expr, &varName, &varValue);
    int ix = get_var_index(varName);
    if (ix == -1) {
        variableList[VAR_NUM] = varName;
        valueList[VAR_NUM] = varValue;
        ++VAR_NUM;
    }
    else {
        free(varName);
        free(valueList[ix]);
        valueList[ix] = varValue;
    }
}
