#include "token_parser.h"
#include "stack.h"
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

        /// Tokens ///

Token makeToken(TokenType tokenType, double num) {
    //something kind of class constructor...
    Token token;
    token.tokenType = tokenType;
    token.val = num;
    return token;
}

List *makeTokenList(String expr) {
    //a valid expression without white spaces is expected
    List *tokenList = NULL;
    TokenType tokenType = NotToken;
    double val = 0.0;
    while (*expr != '\0') {
        if (isdigit(*expr)) {
            val = strtod(expr, &expr);
            tokenType = NumberToken;
        }
        else  {
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
        tokenList = append(tokenList, makeToken(tokenType, val));
    }
    return tokenList;
}

void printTokenList(List *tokenList) {
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
