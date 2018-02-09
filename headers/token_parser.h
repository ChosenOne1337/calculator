#ifndef TOKEN_PARSER_H
#define TOKEN_PARSER_H

typedef struct List List;

typedef enum TokenType {
    NotToken,
    NumberToken,
    PlusOperToken,
    MinusOperToken,
    MultOperToken,
    DivOperToken,
    LeftBracketToken,
    RightBracketToken
} TokenType;

//operators' priorities
static const int operPrty[] = {-1, -1, 3, 3, 4, 4, 1, 2};

typedef struct Token {
    double val;
    TokenType tokenType;
} Token;

Token make_token(TokenType tokenType, double num);
List *make_token_list(char *expr);
void print_token_list(List *tokenList);

#endif // TOKEN_PARSER_H
