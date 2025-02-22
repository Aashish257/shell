#include <stdio.h>
#include <stdlib.h>
#include <direct.h> // Include for _chdir function
#include <windows.h> // Include for GetCurrentDirectory
#include <string.h>
#include "shell.h"

void handle_cd(char **args) {
    if (args[1] == NULL) {
        printf("cd: missing argument\n");
    } else {
        if (_chdir(args[1]) != 0) {
            perror("cd failed");
        }
    }
}

void handle_pwd() {
    char cwd[1024];
    if (_getcwd(cwd, sizeof(cwd)) != NULL) {
        printf("%s\n", cwd);
    } else {
        perror("pwd failed");
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
