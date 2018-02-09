#ifndef ERROR_H
#define ERROR_H

typedef enum ErrorType {
    InvalidExprError ,
    ParenthesesDisbalance,
    DivZeroError,
    UndeclaredVariableError,
    InvalidIdentifierError,
    InvalidValueError,
    MallocError,
    TotalErrors
} ErrorType;

typedef struct Error {
    int isError;
    const char *msg;
} Error;

int is_error();
const char *get_error_msg();
void set_error(ErrorType errorType);
void reset_error();

#endif // ERROR_H
