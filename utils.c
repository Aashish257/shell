#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <direct.h> // Include for _chdir function
#include "shell.h"

int parse_input(char *input, char **args) {
    if (input == NULL || strlen(input) == 0) {
        fprintf(stderr, "Error: No input provided\n");
        return 0; // Return 0 for no arguments
    }

    // Parse input into tokens & check for background execution
    char *token = strtok(input, " ");
    int index = 0;
    int background = 0;

    while (token != NULL) { 
        if (strcmp(token, "") == 0) {
            fprintf(stderr, "Error: Empty command\n");
            return 0; // Return 0 for empty command
        }

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
