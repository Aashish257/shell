#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

#define MAX_INPUT 100

// Function to execute a command
void execute_command(char *command) {
    STARTUPINFO si;
    PROCESS_INFORMATION pi;
    
    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    ZeroMemory(&pi, sizeof(pi));
    
    // Create command string with cmd.exe /c prefix
    char cmd[MAX_INPUT + 10];
    snprintf(cmd, sizeof(cmd), "cmd.exe /c %s", command);
    
    if (!CreateProcess(NULL,   // No module name (use command line)
                       cmd,    // Command line with cmd.exe prefix
                       NULL,    // Process handle not inheritable
                       NULL,    // Thread handle not inheritable
                       FALSE,   // Set handle inheritance to FALSE
                       0,       // No creation flags
                       NULL,    // Use parent's environment block
                       NULL,    // Use parent's starting directory 
                       &si,     // Pointer to STARTUPINFO structure
                       &pi)     // Pointer to PROCESS_INFORMATION structure

    ) {
        printf("Command failed (error: %d)\n", GetLastError());
        printf("DEBUG: Failed command: '%s'\n", command);
        return;
    }
    printf("DEBUG: Successfully executed command: '%s'\n", command);
    
    // Wait until child process exits
    WaitForSingleObject(pi.hProcess, INFINITE);
    
    // Close process and thread handles
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
}

// Function to handle user input
void process_input(char *input) {
    // Remove newline character
    input[strcspn(input, "\n")] = 0;
    
    printf("DEBUG: Received command: '%s'\n", input);
    
    // Handle exit command
    if (strcmp(input, "exit") == 0) {
        exit(0);
    }
    
    // Execute other commands
    printf("DEBUG: Executing command...\n");
    execute_command(input);
}

int main() {
    char input[MAX_INPUT];
    
    while (1) {
        printf("> ");
        fflush(stdout);  // Ensure prompt is displayed
        if (!fgets(input, MAX_INPUT, stdin)) {
            printf("DEBUG: No input received or EOF reached\n");
            break;
        }
        printf("DEBUG: Raw input: '%s'\n", input);  // Show raw input
        process_input(input);
    }

    
    return 0;
}
