#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifdef _WIN32
#include <windows.h>
#include <direct.h> // Include for _chdir function
#endif
#include "shell.h"

/**
 * Parses the input command string into arguments.
 * Detects if the command should be run in the background (if '&' is the last token).
 * @param input The input command string.
 * @param args Array to store parsed arguments.
 * @return 1 if the command should run in the background, 0 otherwise.
 */
int parse_input(char *input, char **args) {
    if (input == NULL || strlen(input) == 0 || strspn(input, " ") == strlen(input)) {
        fprintf(stderr, "Error: No input provided\n");
        return 0; // Return 0 for no arguments
    }

    // Parse input into tokens & check for background execution only if '&' is the last token
    char *token = strtok(input, " ");
    int index = 0;
    int background = 0;
    char *last_token = NULL;

    while (token != NULL) { 
        // Skip empty tokens
        if (strlen(token) == 0) {
            token = strtok(NULL, " ");
            continue;
        }

        last_token = token;

        if (strcmp(token, "&") == 0) {
            // If '&' is not the last token, treat it as a normal argument
            char *next_token = strtok(NULL, " ");
            if (next_token == NULL) {
                background = 1;  // Mark as background process
                break;
            } else {
                args[index++] = token;
                token = next_token;
                continue;
            }
        } else {
            args[index++] = token;
        }
        token = strtok(NULL, " ");
    }
    args[index] = NULL;  // NULL-terminate the argument list

    if (index == 0) {
        fprintf(stderr, "Error: No command entered\n");
    }

    return background; // Return whether it's a background process
}
