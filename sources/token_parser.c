#include "token_parser.h"
#include "stack.h"
#include "calculator.h"
#include "identifiers.h"
#include "error.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

        /// Tokens ///

Token make_token(TokenType tokenType, double num) {
    Token token = {.val = num, .tokenType = tokenType};
    return token;
}

List *make_token_list(char *expr) {
    //a valid expression without white spaces is expected
    List *tokenList = NULL;
    TokenType tokenType = NotToken;
    double val = 0.0;
    char *varName = NULL, *varValue = NULL;
    for (; *expr; tokenList = append(tokenList, make_token(tokenType, val))) {
        if (isdigit(*expr)) {
            val = strtod(expr, &expr);
            tokenType = NumberToken;
            continue;
        }
        if (isalpha(*expr)) {
            varName = extract_var(expr);
            expr += strlen(varName);
            varValue = get_var_value(varName);
            destroy_string(varName);
            if (varValue == NULL) {
                set_error(UndeclaredVariableError);
                break;
            }
            val = strtod(varValue, NULL);
            tokenType = NumberToken;
            continue;
        }
        val = 0.0;
        switch (*expr++) {
        case '+':
            tokenType = PlusOperToken;
            break;
        case '-':
            tokenType = MinusOperToken;
            break;
        case '*':
            tokenType = MultOperToken;
            break;
        case '/':
            tokenType = DivOperToken;
            break;
        case '(':
            tokenType = LeftBracketToken;
            break;
        case ')':
            tokenType = RightBracketToken;
            break;
        default:
            //invalid token
            tokenType = NotToken;
            break;
        }
    }
    return tokenList;
}

void print_token_list(List *tokenList) {
    int ERROR = 0;
    while (tokenList != NULL && !ERROR) {
        switch (tokenList->data.tokenType) {
            case NumberToken:
                printf("%.5lg", tokenList->data.val);
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
                ERROR = 1;
                break;
        }
        tokenList = tokenList->pNext;
        putchar(' ');
    }
    if (ERROR) {
        printf("\nInvalid token list!\n");
    }
}
