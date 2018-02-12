#include <stdlib.h>
#include "calculator.h"
#include "token_parser.h"
#include "identifiers.h"
#include "error.h"
#include "stack.h"

List *transform_to_rpn(List *tokenList);
double calculate_rpn(List *rpnList);
double apply_operator(double op1, double op2, TokenType tokenType);

double calculate(char *expr) {
    //an expression without white spaces is expected
    List *tokenList = NULL, *rpnList = NULL;
    double val = 0.0;
    if (is_empty(expr)) {
        set_error(EmptyExprError);
        return 0.0;
    }
    tokenList = make_token_list(expr);
    if (is_error()) {
        goto free_resources;
    }
    rpnList = transform_to_rpn(tokenList);
    if (is_error()) {
        goto free_resources;
    }
    val = calculate_rpn(rpnList);

    free_resources:
    destroy_list(rpnList);
    destroy_list(tokenList);
    return val;
}

List *transform_to_rpn(List *tokenList) {
    Stack *tokenStack = create_stack();
    List *rpnList = NULL;
    List *curToken = tokenList;
    for (; curToken != NULL; curToken = curToken->pNext) {
        switch (curToken->token.type) {
            case NotToken:
                set_error(InvalidExprError);
                goto free_resources;
            case NumberToken:
                //all the numbers go directly to the rpn expression
                rpnList = append(rpnList, curToken->token);
                break;
            case LeftBracketToken:
                //left brackets go to the stack
                push(tokenStack, curToken->token);
                break;
            case RightBracketToken:
                //extract all operators from the stack until a left bracket is met
                while (!empty(tokenStack) && top(tokenStack).type != LeftBracketToken) {
                    rpnList = append(rpnList, pop(tokenStack));
                }
                //check if a left bracket was met
                if (empty(tokenStack)) {
                    set_error(ParenthesesDisbalance);
                    goto free_resources;
                }
                //get rid of the left bracket
                pop(tokenStack);
                break;
            default:
                // +, -, *, / operators
                //extract operators from the stack while current operator's priority is lower
                //it means that these operators now have their operands in the rpn expression
                while (!empty(tokenStack) &&
                       operPrty[curToken->token.type] <= operPrty[top(tokenStack).type]) {
                    rpnList = append(rpnList, pop(tokenStack));
                }
                //push current operator to the stack
                push(tokenStack, curToken->token);
                break;
        }
    }
    while (!empty(tokenStack)) {
        //if the stack contains a left bracket - there is parentheses disbalance
        if (top(tokenStack).type == LeftBracketToken) {
            set_error(ParenthesesDisbalance);
            goto free_resources;
        }
        //flushing the stack
        rpnList = append(rpnList, pop(tokenStack));
    }

    free_resources:
    destroy_stack(tokenStack);
    return rpnList;
}

double calculate_rpn(List *rpnExpr) {
    Stack *tokenStack = create_stack();
    List *curToken = rpnExpr;
    double val, op1, op2;
    val = op1 = op2 = 0.0;
    for (; curToken != NULL; curToken = curToken->pNext) {
        if (curToken->token.type == NumberToken) {
            push(tokenStack, curToken->token);
            continue;
        }
        if (empty(tokenStack)) {
            set_error(InvalidExprError);
            goto free_resources;
        }
        op2 = pop(tokenStack).val;
        if (empty(tokenStack)) {
            set_error(InvalidExprError);
            goto free_resources;
        }
        op1 = pop(tokenStack).val;
        val = apply_operator(op1, op2, curToken->token.type);
        push(tokenStack, make_token(NumberToken, val));
    }
    if (tokenStack->size != 1) {
        set_error(InvalidExprError);
        goto free_resources;
    }

    free_resources:
    destroy_stack(tokenStack);
    return (is_error()) ? 0.0 : val;
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
