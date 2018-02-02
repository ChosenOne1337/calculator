#ifndef EXPR_STRING_H
#define EXPR_STRING_H

typedef char *String;

String readString(void);
String removeSpaces(String str);
void destroyString(String str);
int isExpr(String expr);

#endif // EXPR_STRING_H
