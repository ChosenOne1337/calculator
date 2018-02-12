#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libgen.h>
#include "calculator.h"

int EXIT_FLAG = 0;

int command_handler(char *expr);
void calc_file(FILE *pInput, FILE *pOutput);
void calc(char *expr, FILE *pOutput);
void print_usage(char *appPath);
void print_info(void);

void calc_file(FILE *pInput, FILE *pOutput) {
    char *expr = NULL;
    while (!feof(pInput) && !EXIT_FLAG) {
        if (pInput == stdin) {
            printf("\n>> ");
            fflush(stdout);
        }
        expr = read_string(pInput);
        remove_spaces(expr);
        if (is_empty(expr)) {
            continue;
        }
        if (command_handler(expr)) {
            continue;
        }
        calc(expr, pOutput);
        destroy_string(expr);
    }
    destroy_var_list();
}

void calc(char *expr, FILE *pOutput) {
    reset_error();
    if (is_var_declaration(expr)) {
        if (is_error()) {
            fprintf(stderr, "%s\n", get_error_msg());
            return;
        }
        add_var(expr);
        return;
    }
    double val = calculate(expr);
    if (is_error()) {
        fprintf(stderr, "%s\n", get_error_msg());
        return;
    }
    fprintf(pOutput, "%s = %.5lg\n", expr, val);
}

int command_handler(char *expr) {
    if (!strcmp(expr, "help")) {
        print_info();
        return 1;
    }
    if (!strcmp(expr, "varlist")) {
        print_var_list();
        return 1;
    }
    if (!strcmp(expr, "exit")) {
        EXIT_FLAG = 1;
        return 1;
    }
    return 0;
}

// command-line interface

typedef enum Option {
    HelpOption,
    FileOption,
    TotalOptions
} Option;

static const char *options[TotalOptions] = {"-h", "-f"};

void print_usage(char *appPath) {
    char *appName = basename(appPath);
    printf("Usage:\n"
           "%s: read from the stdin, write to the stdout\n"
           "%s [-f] file: read from the file, write to the standard output\n"
           "%s [-f] file1 file2: read from the file1, write to the file2\n",
           appName, appName, appName);
}

void print_info(void) {
    printf("Calculator v1.0\n"
           "Command list:\n"
           "*** help - show the help menu\n"
           "*** varlist - print the list of defined variables\n"
           "*** exit or ctrl + Z - close the program\n");
}

int main(int argc, char *argv[]) {
    if (argc <= 1) {
        //stdin
        calc_file(stdin, stdout);
        return 0;
    }
    if (!strcmp(argv[1], options[HelpOption])) {
        //usage information
        print_usage(argv[0]);
        return 0;
    }
    if (!strcmp(argv[1], options[FileOption])) {
        //read from files
        if (argc == 2) {
            print_usage(argv[0]);
            return 0;
        }
        FILE *pInput = fopen(argv[2], "r");
        if (pInput == NULL) {
            fprintf(stderr, "Error: Failed to open %s!", argv[2]);
            return 1;
        }
        if (argc == 3) {
            calc_file(pInput, stdout);
        }
        if (argc >= 4) {
            FILE *pOutput = fopen(argv[3], "w");
            if (pOutput == NULL) {
                fprintf(stderr, "Error: Failed to open %s!", argv[3]);
                fclose(pInput);
                return 1;
            }
            calc_file(pInput, pOutput);
            fclose(pOutput);
        }
        fclose(pInput);
        return 0;
    }
    print_usage(argv[0]);
    return 0;
}
