#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "shell.h"

void execute_command(char **args, int background) {
    if (strcmp(args[0], "exit") == 0) {
        handle_exit();
    } else if (strcmp(args[0], "cd") == 0) {
        handle_cd(args);
    } else if (strcmp(args[0], "pwd") == 0) {
        handle_pwd();
    } else if (strcmp(args[0], "dir") == 0) {
        system("dir"); // Handle dir command

        // Handle other commands
    }
}

void process_input(char *input) {
    char *args[MAX_ARGS];
    int background = parse_input(input, args);
    execute_command(args, background);
}

int main() {
    char input[MAX_INPUT_SIZE];

    while (1) { 
        // Remove the trailing newline character
        input[strcspn(input, "\n")] = 0; 

        printf("MyShell> ");
        if (fgets(input, sizeof(input), stdin) == NULL) {
            break; // Exit on EOF
        }
        process_input(input);
    }

    return 0;
}
