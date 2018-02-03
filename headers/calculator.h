#ifndef CALCULATOR_H
#define CALCULATOR_H

#include "expr_string.h"

typedef enum ErrorType {
    InvalidExprError ,
    ParenthesesDisbalance,
    DivZeroError,
    MallocError
} ErrorType;

typedef struct Error {
    int isError;
    String msg;
} Error;

static const String errMsg[] = {    "Error: Invalid expression!",
                                    "Error: Parentheses disbalance!",
                                    "Error: Division by zero!",
                                    "Error: Memory allocation failure!"};

double calculate(String expr, Error *ptrError);

#endif // CALCULATOR_H
