#include <stdlib.h>
#include "calculator.h"
#include "token_parser.h"
#include "identifiers.h"
#include "error.h"
#include "stack.h"

List *get_rpn(List *tokenList) {
    if (is_error()) {
        return NULL;
    }
    Stack *tokenStack = create_stack();
    List *rpnExpr = NULL;
    List *curToken = tokenList;
    for (; curToken != NULL && !is_error(); curToken = curToken->pNext) {
        switch (curToken->data.tokenType) {
            case NotToken:
                set_error(InvalidExprError);
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
                    set_error(ParenthesesDisbalance);
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
            set_error(ParenthesesDisbalance);
            break;
        }
        rpnExpr = append(rpnExpr, pop(tokenStack));
    }
    destroy_stack(tokenStack);
    return rpnExpr;
}

double apply_operator(double op1, double op2, TokenType tokenType) {
    switch (tokenType) {
        case PlusOperToken:
            return op1 + op2;
        case MinusOperToken:
            return op1 - op2;
        case MultOperToken:
            return op1 * op2;
        case DivOperToken:
            if (op2 == 0.0) {
                set_error(DivZeroError);
                return 0.0;
            }
            return op1 / op2;
        default:
            set_error(InvalidExprError);
            return 0.0;
    }
}

double calculate_rpn(List *rpnExpr) {
    if (is_error()) {
        return 0.0;
    }
    Stack *tokenStack = create_stack();
    List *curToken = rpnExpr;
    double val, op1, op2;
    val = op1 = op2 = 0.0;
    for (; curToken != NULL && !is_error(); curToken = curToken->pNext) {
        if (curToken->data.tokenType == NumberToken) {
            push(tokenStack, curToken->data);
            continue;
        }
        if (empty(tokenStack)) {
            set_error(InvalidExprError);
            break;
        }
        op2 = pop(tokenStack).val;
        if (empty(tokenStack)) {
            set_error(InvalidExprError);
            break;
        }
        op1 = pop(tokenStack).val;
        val = apply_operator(op1, op2, curToken->data.tokenType);
        push(tokenStack, make_token(NumberToken, val));
    }
    if (empty(tokenStack) || tokenStack->size > 1) {
        set_error(InvalidExprError);
    }
    else {
        val = pop(tokenStack).val;
    }
    destroy_stack(tokenStack);
    return (is_error()) ? 0.0 : val;
}

double calculate(char *expr) {
    //an expression without white spaces is expected
    if (expr == NULL) {
        set_error(MallocError);
        return 0.0;
    }
    List *tokenList = make_token_list(expr);
    if (is_error()) {
        destroy_list(tokenList);
        return 0.0;
    }
    List *rpnExpr = get_rpn(tokenList);
    double val = calculate_rpn(rpnExpr);
    destroy_list(rpnExpr);
    destroy_list(tokenList);
    return val;
}
