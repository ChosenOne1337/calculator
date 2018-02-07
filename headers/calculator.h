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
    const char *msg;
} Error;

double calculate(const char *expr, Error *ptrError);

#endif // CALCULATOR_H
