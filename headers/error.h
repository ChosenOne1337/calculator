#ifndef ERROR_H
#define ERROR_H

typedef enum ErrorType {
    InvalidExprError ,
    ParenthesesDisbalance,
    DivZeroError,
    UndeclaredVariableError,
    InvalidIdentifierError,
    InvalidValueError,
    EmptyExprError,
    TotalErrors
} ErrorType;

typedef struct Error {
    int isError;
    const char *msg;
} Error;

int is_error(void);
const char *get_error_msg(void);
void set_error(ErrorType errorType);
void reset_error(void);

#endif // ERROR_H
