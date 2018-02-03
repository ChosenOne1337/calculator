#include <stdio.h>
#include <stdlib.h>
#include "calculator.h"

int main(void)
{
    String expr = NULL;
    Error error = {0, NULL};
    int loop = 1;
    double val = 0.0;
    while (loop) {
        system("cls");
        printf("Enter an arithmetic expression:\nEXPR: ");
        expr = readString();
        val = calculate(expr, &error);
        if (!error.isError) {
            printf("ANSWER: %.5lg", val);
        }
        else {
            fprintf(stderr, "%s\n", error.msg);
        }
        destroyString(expr);
        printf("\nTry again?\n");
        scanf("%d", &loop);
        getchar(); //discards a trailing newline character
    }
    return 0;
}
