#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "token_parser.h"
#include "stack.h"
#include "identifiers.h"
#include "error.h"

double value_from_identifer(char *expr, char **endPtr);
char *extract_identifier(char *expr);
TokenType identify_oper_type(char c);

Token make_token(TokenType type, double num) {
    Token token = {.val = num, .type = type};
    return token;
}

List *make_token_list(char *expr) {
    //a valid expression without white spaces is expected
    List *tokenList = NULL;
    TokenType tokenType = NotToken;
    double val = 0.0;
    for (; *expr; tokenList = append(tokenList, make_token(tokenType, val))) {
        if (isdigit(*expr)) {
            //a number
            val = strtod(expr, &expr);
            tokenType = NumberToken;
            continue;
        }
        if (isalpha(*expr)) {
            //a possible variable
            val = value_from_identifer(expr, &expr);
            tokenType = NumberToken;
            if (is_error()) {
                break;
            }
            continue;
        }
        //a possible operator
        val = 0.0;
        tokenType = identify_oper_type(*expr++);
    }
    return tokenList;
}

double value_from_identifer(char *expr, char **endPtr) {
    char *varName = extract_identifier(expr);
    *endPtr += strlen(varName);
    char *varValue = get_var_value(varName);
    free(varName);
    if (varValue == NULL) {
        set_error(UndeclaredVariableError);
        return 0.0;
    }
    double val = strtod(varValue, NULL);
    return val;
}

char *extract_identifier(char *expr) {
    //reads an identifier from expr,
    //then substitutes it with a value if it's possible
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

TokenType identify_oper_type(char c) {
    switch (c) {
    case '+':
        return PlusOperToken;
    case '-':
        return MinusOperToken;
    case '*':
        return MultOperToken;
    case '/':
        return DivOperToken;
    case '(':
        return LeftBracketToken;
    case ')':
        return RightBracketToken;
    default:
        return NotToken;
    }
}

void print_token_list(List *tokenList) {
    while (tokenList != NULL) {
        switch (tokenList->token.type) {
            case NumberToken:
                printf("%.5lg", tokenList->token.val);
                break;
            case PlusOperToken:
                putchar('+');
                break;
            case MinusOperToken:
                putchar('-');
                break;
            case MultOperToken:
                putchar('*');
                break;
            case DivOperToken:
                putchar('/');
                break;
            case LeftBracketToken:
                putchar('(');
                break;
            case RightBracketToken:
                putchar(')');
                break;
            case NotToken:
                putchar('\n');
                return;
        }
        tokenList = tokenList->pNext;
        putchar(' ');
    }
}
