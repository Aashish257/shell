#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include "shell.h"

void execute_command(char **args, int background) {
    STARTUPINFO si;
    PROCESS_INFORMATION pi;
    char command[1024] = "cmd.exe /c "; // Use Windows Command Prompt
    size_t command_length = strlen(command);
    
    // Convert args[] array into a single command string
    for (int i = 0; args[i] != NULL; i++) {
        if (command_length + strlen(args[i]) + 1 < sizeof(command)) {
            strcat(command, args[i]);
            strcat(command, " ");
            command_length += strlen(args[i]) + 1; // Update command length
        } else {
            fprintf(stderr, "Error: Command is too long to execute.\n");
            return;
        }
    }

    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    ZeroMemory(&pi, sizeof(pi));

    // Create a new process for the command
    if (!CreateProcess(NULL, command, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi)) {
        fprintf(stderr, "Error: Command execution failed for command: %s\n", command);
        return;
    }

    if (background) {
        CreateProcess(NULL, command, NULL, NULL, FALSE, CREATE_NO_WINDOW, NULL, NULL, &si, &pi);
    } else {
        WaitForSingleObject(pi.hProcess, INFINITE);
    }

    // Close process handles
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
}

void process_input(char *input) {
    char *args[MAX_ARGS];
    int background = parse_input(input, args);
    
    // Check for exit command
    if (args[0] != NULL && strcmp(args[0], "exit") == 0) {
        handle_exit(); // Call handle_exit to terminate the shell
        return;
    }
    
    execute_command(args, background);
}


int main() {
    char input[MAX_INPUT_SIZE];

    while (1) { 
        // Remove the trailing newline character
        input[strcspn(input, "\n")] = 0; 

        printf("> "); // Prompt for user input

        if (fgets(input, sizeof(input), stdin) == NULL) {
            break; // Exit on EOF
        }
        process_input(input);
    }

    return 0;
}
