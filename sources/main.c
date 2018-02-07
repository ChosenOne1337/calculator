#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libgen.h>
#include "calculator.h"

void calc(FILE *pInput, FILE *pOutput) {
    Error error = {.isError = 0, .msg = NULL};
    char *str = readString(pInput);
    double val = calculate(str, &error);
    destroyString(str);
    if (error.isError) {
        fprintf(stderr, "%s\n", error.msg);
        return;
    }
    fprintf(pOutput, "%.5lg\n", val);
}

void calcFile(FILE *pInput, FILE *pOutput) {
    if (pInput == stdin) {
        calc(stdin, pOutput);
        return;
    }
    while (!feof(pInput)) {
        calc(pInput, pOutput);
    }
}

// command-line interface

typedef enum Option {
    HelpOption,
    FileOption,
    TotalOptions
} Option;

static const char *options[TotalOptions] = {"-h", "-f"};

void printUsage(char *appPath) {
    char *appName = basename(appPath);
    printf("Usage:\n"
           "%s: read expression from the standard input,\n"
           "%s [-f] f1 f2: read expressions from the f1, put results to the f2\n",
           appName, appName);
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
    if (!strcmp(argv[1], options[FileOption])) {
        //read from files
        if (argc < 4) {
            printUsage(argv[0]);
            return 0;
        }
        FILE *pInput = fopen(argv[2], "r");
        if (pInput == NULL) {
            fprintf(stderr, "Error: Failed to open %s!", argv[2]);
            return 1;
        }
        FILE *pOutput = fopen(argv[3], "w");
        if (pOutput == NULL) {
            fprintf(stderr, "Error: Failed to open %s!", argv[3]);
            return 1;
        }
        calcFile(pInput, pOutput);
        fclose(pInput);
        fclose(pOutput);
        return 0;
    }
    printUsage(argv[0]);
    return 0;
}
