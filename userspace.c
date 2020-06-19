#include <ctype.h>
#include <float.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <unistd.h>
#include <sys/reboot.h>

#define MAXIMAL_INPUT_LENGTH 16384

#define DELIMITER 0
#define NUMBER 1

#define ERROR_OK 0
#define ERROR_ZERO 1
#define ERROR_BRACKETS 2
#define ERROR_SYNTAX 3
#define ERROR_SYMBOL 4

#define NEWLINE "\033c"

#define BLACK "\033[1;30m"
#define RED "\033[1;31m"
#define GREEN "\033[1;32m"
#define YELLOW "\033[1;33m"
#define BLUE "\033[1;34m"
#define MAGENTA "\033[1;35m"
#define CYAN "\033[1;36m"
#define WHITE "\033[1;37m"

static const char *error_list[] = {
    "OK",
    "Division by zero",
    "Unbalanced brackets",
    "Invalid tokens order",
    "Unknown symbol",
};

static int token_type;
static int error;
static char input[MAXIMAL_INPUT_LENGTH];
static char token[MAXIMAL_INPUT_LENGTH];
static char *input_ptr;

static void calculate_1(double *answer);
static void calculate_2(double *answer);
static void calculate_3(double *answer);
static void calculate_4(double *answer);
static void calculate_5(double *answer);
static void calculate_6(double *answer);

static char worst_error(char current_error) {
    if (error < current_error) {
        return current_error;
    }
    return error;
}

static bool isdelimiter(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' ||
           c == '^' || c == '(' || c == ')' || c == '.' || c == '\0';
}

static void get_token() {
    char *token_ptr = token;
    while (isblank(*input_ptr)) {
        ++input_ptr;
    }
    if (isdelimiter(*input_ptr)) {
        token_type = DELIMITER;
        *token_ptr++ = *input_ptr++;
    } else if (isdigit(*input_ptr)) {
        token_type = NUMBER;
        while (isdigit(*input_ptr)) {
            *token_ptr++ = *input_ptr++;
        }
        if (*input_ptr == '.') {
            *token_ptr++ = *input_ptr++;
            while (isdigit(*input_ptr)) {
                *token_ptr++ = *input_ptr++;
            }
        }
    } else {
        error = worst_error(ERROR_SYMBOL);
    }
    *token_ptr = '\0';
}

static void calculate_1(double *answer) {
    calculate_2(answer);
    char operation = *token;
    double tmp;
    while (operation == '+' || operation == '-') {
        get_token();
        calculate_2(&tmp);
        if (operation == '+') {
            *answer += tmp;
        } else {
            *answer -= tmp;
        }
        operation = *token;
    }
}

static void calculate_2(double *answer) {
    calculate_3(answer);
    char operation = *token;
    double tmp;
    while (operation == '*' || operation == '/') {
        get_token();
        calculate_3(&tmp);
        if (operation == '*') {
            *answer *= tmp;
        } else if (fabs(tmp) >= DBL_EPSILON) {
            *answer /= tmp;
        } else {
            error = worst_error(ERROR_ZERO);
        }
        operation = *token;
    }
}

static void calculate_3(double *answer) {
    double tmp;
    calculate_4(answer);
    if (*token == '^') {
        get_token();
        calculate_3(&tmp);
        *answer = pow(*answer, tmp);
    }
}

static void calculate_4(double *answer) {
    char operation = 0;
    if (*token == '+' || *token == '-') {
        operation = *token;
        get_token();
    }
    calculate_5(answer);
    if (operation == '-') {
        *answer = -*answer;
    }
}

static void calculate_5(double *answer) {
    if (*token == '(') {
        get_token();
        calculate_1(answer);
        if (*token == ')') {
            get_token();
        } else {
            error = worst_error(ERROR_BRACKETS);
        }
    } else {
        calculate_6(answer);
    }
}

static void calculate_6(double *answer) {
    if (token_type == NUMBER) {
        *answer = atof(token);
        get_token();
    } else {
        error = worst_error(ERROR_SYNTAX);
    }
}

static void clear() {
    fputs(NEWLINE, stdout);
    fflush(stdout);
}

static void help() {
    fputs(MAGENTA "Welcome to Calculinux 1.1!\n"
    MAGENTA "Type an expression and get the result\n"
    MAGENTA "For example: " YELLOW "1 / 2 + 3 * 2 ^ -2\n" MAGENTA
    "Type " YELLOW "clear" MAGENTA " to clear the screen\n"
    "Type " YELLOW "help" MAGENTA " to print this message again\n"
    "Type " YELLOW "poweroff" MAGENTA " to power off the computer\n"
    "Type " YELLOW "reboot" MAGENTA " to reboot\n", stdout);
    fflush(stdout);
}

static void linux_poweroff() {
    reboot(RB_POWER_OFF);
}

static void linux_reboot() {
    reboot(RB_AUTOBOOT);
}

int main() {
    clear();
    help();
    while (!feof(stdin)) {
        double answer = 0;
        bool input_empty = false;
        error = ERROR_OK;
        *input = '\0';
        input_ptr = input;
        fputs(BLUE ">>> " GREEN, stdout);
        fflush(stdout);
        fgets(input, MAXIMAL_INPUT_LENGTH, stdin);
        if (input[strlen(input) - 1] == '\n') {
            input[strlen(input) - 1] = '\0';
        } else {
            fputs("\n", stdout);
            fflush(stdout);
        }
        if (strcmp(input, "clear") == 0) {
            clear();
        } else if (strcmp(input, "help") == 0) {
            help();
        } else if (strcmp(input, "poweroff") == 0) {
            linux_poweroff();
        } else if (strcmp(input, "reboot") == 0) {
            linux_reboot();
        } else {
            get_token();
            if (!*token) {
                input_empty = true;
            } else {
                calculate_1(&answer);
                if (*token) {
                    error = worst_error(ERROR_SYNTAX);
                }
            }
            if (error) {
                printf(RED "%s\n", error_list[error]);
                fflush(stdout);
            } else if (!input_empty) {
                printf(GREEN "%lf\n", answer);
                fflush(stdout);
            }
        }
    }
    linux_poweroff();
    return 0;
}
