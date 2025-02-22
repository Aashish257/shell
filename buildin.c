#include <stdio.h>
#include <stdlib.h>
#include <direct.h> // Include for _chdir function
#include <windows.h> // Include for GetCurrentDirectory
#include <string.h>
#include "shell.h"

void handle_cd(char **args) {
    // Debug: Log the command being executed
    printf("Executing command: cd with argument: %s\n", args[1]);

    if (args[1] == NULL) {
        fprintf(stderr, "cd: missing argument\n");
    } else {
        if (_chdir(args[1]) != 0) { // Use _chdir for Windows compatibility
            perror("cd");
        }
    }
}

void handle_pwd() {
    // Debug: Log the command being executed
    printf("Executing command: pwd\n");

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

// Function to check and execute built-in commands
int execute_builtin(char **args) {
    if (strcmp(args[0], "cd") == 0) {
        handle_cd(args);
        return 1;
    } 
    if (strcmp(args[0], "pwd") == 0) {
        handle_pwd();
        return 1;
    } 
    if (strcmp(args[0], "exit") == 0) {
        handle_exit();
        return 1;
    }
    return 0;  // Not a built-in command
}
