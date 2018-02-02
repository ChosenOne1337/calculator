#include "calculator.h"
#include "token_parser.h"
#include <stack.h>
#include <stdlib.h>
#include <stdio.h>

List *getRPN(List *tokenList, int *ERROR_BITS) {
    if (*ERROR_BITS) {
        return NULL;
    }
    Stack *tokenStack = createStack();
    List *RPN_expr = NULL;
    List *curToken = tokenList;
    while (curToken != NULL && !*ERROR_BITS) {
        switch (curToken->data.tokenType) {
            case NotToken:
                *ERROR_BITS |= InvalidExprError;
                break;

            case NumberToken:
                RPN_expr = append(RPN_expr, curToken->data);
                break;

            case LeftBracketToken:
                push(tokenStack, curToken->data);
                break;

            case RightBracketToken: {
                while (!empty(tokenStack) && top(tokenStack).tokenType != LeftBracketToken) {
                    RPN_expr = append(RPN_expr, pop(tokenStack));
                }
                if (empty(tokenStack)) {
                    *ERROR_BITS |= ParenthesesDisbalance;
                }
                else {
                    pop(tokenStack);
                }
            } break;

            default: {
                // +, -, *, / operators
                while (!empty(tokenStack) &&
                       operPrty[curToken->data.tokenType] <= operPrty[top(tokenStack).tokenType]) {
                    RPN_expr = append(RPN_expr, pop(tokenStack));
                }
                push(tokenStack, curToken->data);
            } break;
        }
        curToken = curToken->pNext;
    }
    while (!empty(tokenStack)) {
        if (top(tokenStack).tokenType == LeftBracketToken) {
            *ERROR_BITS |= ParenthesesDisbalance;
            break;
        }
        RPN_expr = append(RPN_expr, pop(tokenStack));
    }
    destroyStack(tokenStack);
    return RPN_expr;
}

double calculateRPN(List *RPN_expr, int *ERROR_BITS) {
    if (*ERROR_BITS) {
        return 0.0;
    }
    Stack *tokenStack = createStack();
    List *curToken = RPN_expr;
    double val, op1, op2;
    val = op1 = op2 = 0.0;
    while (curToken != NULL && !*ERROR_BITS) {
        if (curToken->data.tokenType == NumberToken) {
            push(tokenStack, curToken->data);
        }
        else {
            if (empty(tokenStack)) {
                *ERROR_BITS |= InvalidExprError;
                break;
            }
            op1 = pop(tokenStack).val;
            if (empty(tokenStack)) {
                *ERROR_BITS |= InvalidExprError;
                break;
            }
            op2 = pop(tokenStack).val;
            switch (curToken->data.tokenType) {
                case PlusOperToken:
                    val = op2 + op1;
                    break;
                case MinusOperToken:
                    val = op2 - op1;
                    break;
                case MultOperToken:
                    val = op2 * op1;
                    break;
                case DivOperToken:
                    if (op1 == 0.0) {
                        *ERROR_BITS |= DivZeroError;
                        break;
                    }
                    val = op2 / op1;
                    break;
                default:
                    *ERROR_BITS |= InvalidExprError;
                    break;
            }
            push(tokenStack, makeToken(NumberToken, val));
        }
        curToken = curToken->pNext;
    }
    if (empty(tokenStack) || tokenStack->size > 1) {
        *ERROR_BITS |= InvalidExprError;
    }
    else {
        val = pop(tokenStack).val;
    }
    destroyStack(tokenStack);
    return (*ERROR_BITS) ? 0.0 : val;
}

double calculate(String expr, int *ERROR_BITS) {
    double val = 0.0;
    *ERROR_BITS = 0;
    if (expr == NULL) {
        *ERROR_BITS |= MallocError;
        return 0.0;
    }
    String expr_glued = removeSpaces(expr);
    if (expr_glued == NULL) {
        *ERROR_BITS |= MallocError;
        return 0.0;
    }
    if (!isExpr(expr_glued)) {
        *ERROR_BITS |= InvalidExprError;
    }
    else {
        List *tokenList = makeTokenList(expr_glued);
        if (tokenList == NULL) {
            *ERROR_BITS |= InvalidExprError;
        }
        List *RPN_expr = getRPN(tokenList, ERROR_BITS);
        printf("RPN: ");
        printTokenList(RPN_expr);
        printf("\n");
        val = calculateRPN(RPN_expr, ERROR_BITS);
        destroyList(tokenList);
        destroyList(RPN_expr);
    }
    destroyString(expr_glued);
    return val;
}
