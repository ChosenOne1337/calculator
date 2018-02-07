#include "calculator.h"
#include "token_parser.h"
#include <stack.h>
#include <stdlib.h>
#include <stdio.h>

static const char *errMsg[] = {    "Error: Invalid expression!",
                                    "Error: Parentheses disbalance!",
                                    "Error: Division by zero!",
                                    "Error: Memory allocation failure!"};

void setError(Error *ptrError, ErrorType errorType) {
    if (ptrError) {
        ptrError->isError = 1;
        ptrError->msg = errMsg[errorType];
    }
}

List *get_rpn(List *tokenList, Error *ptrError) {
    if (ptrError->isError) {
        return NULL;
    }
    Stack *tokenStack = createStack();
    List *rpnExpr = NULL;
    List *curToken = tokenList;
    for (; curToken != NULL && !ptrError->isError; curToken = curToken->pNext) {
        switch (curToken->data.tokenType) {
            case NotToken:
                setError(ptrError, InvalidExprError);
                break;
            case NumberToken:
                rpnExpr = append(rpnExpr, curToken->data);
                break;
            case LeftBracketToken:
                push(tokenStack, curToken->data);
                break;
            case RightBracketToken:
                while (!empty(tokenStack) && top(tokenStack).tokenType != LeftBracketToken) {
                    rpnExpr = append(rpnExpr, pop(tokenStack));
                }
                if (empty(tokenStack)) {
                    setError(ptrError, ParenthesesDisbalance);
                    break;
                }
                pop(tokenStack);
                break;
            default:
                // +, -, *, / operators
                while (!empty(tokenStack) &&
                       operPrty[curToken->data.tokenType] <= operPrty[top(tokenStack).tokenType]) {
                    rpnExpr = append(rpnExpr, pop(tokenStack));
                }
                push(tokenStack, curToken->data);
                break;
        }
    }
    while (!empty(tokenStack)) {
        if (top(tokenStack).tokenType == LeftBracketToken) {
            setError(ptrError, ParenthesesDisbalance);
            break;
        }
        rpnExpr = append(rpnExpr, pop(tokenStack));
    }
    destroyStack(tokenStack);
    return rpnExpr;
}

double applyOperator(double op1, double op2, TokenType tokenType, Error *ptrError) {
    switch (tokenType) {
        case PlusOperToken:
            return op1 + op2;
        case MinusOperToken:
            return op1 - op2;
        case MultOperToken:
            return op1 * op2;
        case DivOperToken:
            if (op2 == 0.0) {
                setError(ptrError, DivZeroError);
                return 0.0;
            }
            return op1 / op2;
        default:
            setError(ptrError, InvalidExprError);
            return 0.0;
    }
}

double calculate_rpn(List *rpnExpr, Error *ptrError) {
    if (ptrError->isError) {
        return 0.0;
    }
    Stack *tokenStack = createStack();
    List *curToken = rpnExpr;
    double val, op1, op2;
    val = op1 = op2 = 0.0;
    for (; curToken != NULL && !ptrError->isError; curToken = curToken->pNext) {
        if (curToken->data.tokenType == NumberToken) {
            push(tokenStack, curToken->data);
            continue;
        }
        if (empty(tokenStack)) {
            setError(ptrError, InvalidExprError);
            break;
        }
        op2 = pop(tokenStack).val;
        if (empty(tokenStack)) {
            setError(ptrError, InvalidExprError);
            break;
        }
        op1 = pop(tokenStack).val;
        val = applyOperator(op1, op2, curToken->data.tokenType, ptrError);
        push(tokenStack, makeToken(NumberToken, val));
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

double calculate(char *expr, Error *ptrError) {
    //an expression without white spaces is expected
    double val = 0.0;
    ptrError->isError = 0;
    if (expr == NULL) {
        setError(ptrError, MallocError);
        return 0.0;
    }
    List *tokenList = makeTokenList(expr);
    if (tokenList == NULL) {
        setError(ptrError, InvalidExprError);
    }
    List *rpnExpr = get_rpn(tokenList, ptrError);
    val = calculate_rpn(rpnExpr, ptrError);
    destroyList(tokenList);
    destroyList(rpnExpr);
    return val;
}
