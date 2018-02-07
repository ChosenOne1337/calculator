#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libgen.h>
#include "calculator.h"

void calc(char *expr, FILE *pOutput) {
    Error error = {.isError = 0, .msg = NULL};
    expr = removeSpaces(expr);
    if (expr[0] == '\0') {
        return;
    }
    double val = calculate(expr, &error);
    if (error.isError) {
        fprintf(stderr, "%s\n", error.msg);
        return;
    }
    fprintf(pOutput, "%s = %.5lg\n", expr, val);
}

void calcFile(FILE *pInput, FILE *pOutput) {
    char *expr = NULL;
    while (!feof(pInput)) {
        if (pInput == stdin) {
            printf("\n>> ");
            fflush(stdout);
        }
        if ((expr = readString(pInput)) == NULL) {
            continue;
        }
        calc(expr, pOutput);
        destroyString(expr);
    }
}

// command-line interface

typedef enum Option {
    HelpOption,
    InfoOption,
    FileOption,
    TotalOptions
} Option;

static const char *options[TotalOptions] = {"-h", "-i", "-f"};

void printUsage(char *appPath) {
    char *appName = basename(appPath);
    printf("Usage:\n"
           "%s [i]: information about the application\n"
           "%s: read from the stdin, write to the stdout\n"
           "%s [-f] file: read from the file, write to the standard output\n"
           "%s [-f] file1 file2: read from the file1, write to the file2\n",
           appName, appName, appName, appName);
}

void printInfo() {
    printf("Calculator v1.0\n");
}

int main(int argc, char *argv[]) {
    if (argc <= 1) {
        //stdin
        calcFile(stdin, stdout);
        return 0;
    }
    if (!strcmp(argv[1], options[HelpOption])) {
        //usage information
        printUsage(argv[0]);
        return 0;
    }
    if (!strcmp(argv[1], options[InfoOption])) {
        //usage information
        printInfo();
        return 0;
    }
    if (!strcmp(argv[1], options[FileOption])) {
        //read from files
        if (argc == 2) {
            printUsage(argv[0]);
            return 0;
        }
        FILE *pInput = fopen(argv[2], "r");
        if (pInput == NULL) {
            fprintf(stderr, "Error: Failed to open %s!", argv[2]);
            return 1;
        }
        if (argc == 3) {
            calcFile(pInput, stdout);
        }
        if (argc >= 4) {
            FILE *pOutput = fopen(argv[3], "w");
            if (pOutput == NULL) {
                fprintf(stderr, "Error: Failed to open %s!", argv[3]);
                fclose(pInput);
                return 1;
            }
            calcFile(pInput, pOutput);
            fclose(pOutput);
        }
        fclose(pInput);
        return 0;
    }
    printUsage(argv[0]);
    return 0;
}
