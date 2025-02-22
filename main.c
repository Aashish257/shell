#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include "shell.h"


void execute_command(char **args, int background) {
    // Debug: Log the command being executed
    printf("Executing command: %s\n", args[0]);
    
    if (execute_builtin(args)) {
        return; // If it's a built-in command, return early
    }


    // Handle other commands
    STARTUPINFO si;
    PROCESS_INFORMATION pi;
    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    ZeroMemory(&pi, sizeof(pi));

    // Create the command string with arguments
    char command[MAX_INPUT_SIZE];
    snprintf(command, sizeof(command), "%s", args[0]);
    for (int i = 1; args[i] != NULL; i++) {
        strcat(command, " ");
        strcat(command, args[i]);
    }


    // Check for Windows-specific commands
    if (strcmp(args[0], "ls") == 0) {
        strcpy(args[0], "dir"); // Change 'ls' to 'dir'
    }
    // Create a new process to execute the command
    // Debug: Log the constructed command string
    printf("Constructed command: %s\n", command);

    // Ensure the command is valid

    // Ensure the command is valid

    if (!CreateProcess(NULL, command, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi)) {
        fprintf(stderr, "Error executing command: %s\n", args[0]);
    } else {
        // Wait for the process to finish if not background
        if (!background) {
            WaitForSingleObject(pi.hProcess, INFINITE);
        }
        CloseHandle(pi.hProcess);
        CloseHandle(pi.hThread);
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

    printf("MyShell> "); // Prompt for user input

        if (fgets(input, sizeof(input), stdin) == NULL) {
            break; // Exit on EOF
        }
        process_input(input);
    }

    return 0;
}
