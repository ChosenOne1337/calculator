#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "calculator.h"

double calc(const char *expr, FILE *file) {
    Error error = {0, NULL};
    double val = 0.0;
    if (expr == NULL) {
        if (file == NULL) {
            return 0.0;
        }
        char *str = readString(file);
        val = calculate(str, &error);
        destroyString(str);
    }
    else {
        val = calculate(expr, &error);
    }
    if (error.isError) {
        fprintf(stderr, "%s\n", error.msg);
    }
    return val;
}

const char *getFileName(const char *path) {
    //returns pointer to the beginning of the file name
    char *ptr = strrchr(path, '/');
    if (ptr == NULL) {
        ptr = strrchr(path, '\\');
        if (ptr == NULL) {
            return path;
        }
    }
    return ptr + 1;
}

int main(int argc, char *argv[]) {
    if (argc <= 1) {
        //read from the stdin
        printf("%.5lg", calc(NULL, stdin));
        return 0;
    }
    if (!strcmp(argv[1], "-f") && argc >= 4) {
        //file mode
        FILE *pInput = fopen(argv[2], "r");
        FILE *pOutput = fopen(argv[3], "w");
        if (pInput == NULL) {
            printf("Failed to open %s!", argv[2]);
            return 1;
        }
        if (pOutput == NULL) {
            printf("Failed to open %s!", argv[3]);
            return 1;
        }
        while (!feof(pInput)) {
            fprintf(pOutput, "%.5lg\n", calc(NULL, pInput));
        }
        fclose(pInput);
        fclose(pOutput);
        return 0;
    }
    if (argc == 2 && strcmp(argv[1], "-h") != 0) {
        //expression
        printf("%.5lg", calc(argv[1], NULL));
        return 0;
    }
    //-h, --help or incorrect argument list
    char *appName = getFileName(argv[0]);
    printf("Usage:\n"
           "%s: read expression from the standard input,\n"
           "%s \"expr\": calculate expression in quotes,\n"
           "%s [-f] f1 f2: read expressions from the f1, put results to the f2\n",
           appName, appName, appName);
    return 0;
}
