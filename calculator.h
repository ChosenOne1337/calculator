#ifndef CALCULATOR_H
#define CALCULATOR_H

#include "expr_string.h"

typedef enum ErrorTypes {
    InvalidExprError = 1,
    ParenthesesDisbalance = 2,
    DivZeroError = 4,
    MallocError = 8,
} ErrorTypes;

double calculate(String expr, int *ERROR_BITS);

#endif // CALCULATOR_H
