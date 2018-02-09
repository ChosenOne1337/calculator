#include "error.h"
#include <stdlib.h>

static Error ERROR = {.isError = 0, .msg = NULL};

static const char *errMsg[TotalErrors] = {  "Error: Invalid expression!",
                                            "Error: Parentheses disbalance!",
                                            "Error: Division by zero!",
                                            "Error: The expression has an undeclared variable!",
                                            "Error: Invalid identifier!",
                                            "Error: Invalid variable value!",
                                            "Error: Memory allocation failure!"};

void set_error(ErrorType errorType) {
    ERROR.isError = 1;
    ERROR.msg = errMsg[errorType];
}

int is_error() {
    return ERROR.isError;
}

const char *get_error_msg() {
    return ERROR.msg;
}

void reset_error() {
    ERROR.isError = 0;
    ERROR.msg = NULL;
}
