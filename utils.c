#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "shell.h"

// Parse input into tokens & check for background execution
int parse_input(char *input, char **args) {
    char *token = strtok(input, " ");
    int index = 0;
    int background = 0;

    while (token != NULL) {
        if (strcmp(token, "&") == 0) {
            background = 1;  // Mark as background process
        } else {
            args[index++] = token;
        }
        token = strtok(NULL, " ");
    }
    args[index] = NULL;  // NULL-terminate the argument list

    return background; // Return whether it's a background process
}
