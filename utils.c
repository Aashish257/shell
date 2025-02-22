#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <direct.h> // Include for _chdir function

#include "shell.h"

int parse_input(char *input, char **args) {
    // Parse input into tokens & check for background execution
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

void handle_cd(char **args) {
    if (args[1] == NULL) {
        fprintf(stderr, "cd: missing argument\n");
    } else {
        if (_chdir(args[1]) != 0) { // Use _chdir for Windows compatibility
            perror("cd");
        }
    }
}

void handle_pwd() {
    char cwd[MAX_INPUT_SIZE];
    if (GetCurrentDirectory(MAX_INPUT_SIZE, cwd) != 0) {
        printf("%s\n", cwd);
    } else {
        perror("pwd");
    }
}

void handle_exit() {
    printf("Exiting shell...\n");
    exit(0);
}
