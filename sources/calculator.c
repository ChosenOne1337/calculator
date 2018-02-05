#include "calculator.h"
#include "token_parser.h"
#include <stack.h>
#include <stdlib.h>
#include <stdio.h>

void setError(Error *ptrError, ErrorType errorType) {
    if (ptrError) {
        ptrError->isError = 1;
        ptrError->msg = errMsg[errorType];
    }
}

List *getRPN(List *tokenList, Error *ptrError) {
    if (ptrError->isError) {
        return NULL;
    }
    Stack *tokenStack = createStack();
    List *RPN_expr = NULL;
    List *curToken = tokenList;
    while (curToken != NULL && !ptrError->isError) {
        switch (curToken->data.tokenType) {
            case NotToken:
                setError(ptrError, InvalidExprError);
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
                    setError(ptrError, ParenthesesDisbalance);
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
            setError(ptrError, ParenthesesDisbalance);
            break;
        }
        RPN_expr = append(RPN_expr, pop(tokenStack));
    }
    destroyStack(tokenStack);
    return RPN_expr;
}

double calculateRPN(List *RPN_expr, Error *ptrError) {
    if (ptrError->isError) {
        return 0.0;
    }
    Stack *tokenStack = createStack();
    List *curToken = RPN_expr;
    double val, op1, op2;
    val = op1 = op2 = 0.0;
    while (curToken != NULL && !ptrError->isError) {
        if (curToken->data.tokenType == NumberToken) {
            push(tokenStack, curToken->data);
        }
        else {
            if (empty(tokenStack)) {
                setError(ptrError, InvalidExprError);
                break;
            }
            op1 = pop(tokenStack).val;
            if (empty(tokenStack)) {
                setError(ptrError, InvalidExprError);
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
                        setError(ptrError, DivZeroError);
                        break;
                    }
                    val = op2 / op1;
                    break;
                default:
                    setError(ptrError, InvalidExprError);
                    break;
            }
            push(tokenStack, makeToken(NumberToken, val));
        }
        curToken = curToken->pNext;
    }
    if (empty(tokenStack) || tokenStack->size > 1) {
        setError(ptrError, InvalidExprError);
    }
    else {
        val = pop(tokenStack).val;
    }
    destroyStack(tokenStack);
    return (ptrError->isError) ? 0.0 : val;
}

double calculate(const char *expr, Error *ptrError) {
    double val = 0.0;
    ptrError->isError = 0;
    if (expr == NULL) {
        setError(ptrError, MallocError);
        return 0.0;
    }
    char *expr_glued = removeSpaces(expr);
    if (expr_glued == NULL) {
        setError(ptrError, MallocError);
        return 0.0;
    }
    if (!isExpr(expr_glued)) {
        setError(ptrError, InvalidExprError);
    }
    else {
        List *tokenList = makeTokenList(expr_glued);
        if (tokenList == NULL) {
            setError(ptrError, InvalidExprError);
        }
        List *RPN_expr = getRPN(tokenList, ptrError);
        val = calculateRPN(RPN_expr, ptrError);
        destroyList(tokenList);
        destroyList(RPN_expr);
    }
    destroyString(expr_glued);
    return val;
}
