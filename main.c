#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <synchapi.h> // For WaitForSingleObject and INFINITE
#endif

#include "shell.h"

#ifdef _WIN32

Job jobs[MAX_JOBS];
int job_count = 0;

void add_job(HANDLE hProcess, HANDLE hThread, DWORD pid, const char *command) {
    if (job_count >= MAX_JOBS) {
        fprintf(stderr, "Job list full, cannot add more jobs.\n");
        return;
    }
    jobs[job_count].job_id = job_count + 1;
    jobs[job_count].hProcess = hProcess;
    jobs[job_count].hThread = hThread;
    jobs[job_count].pid = pid;
    strncpy(jobs[job_count].command, command, MAX_INPUT_SIZE - 1);
    jobs[job_count].command[MAX_INPUT_SIZE - 1] = '\0';
    jobs[job_count].running = 1;
    job_count++;
}

int remove_job(int job_id) {
    for (int i = 0; i < job_count; i++) {
        if (jobs[i].job_id == job_id) {
            CloseHandle(jobs[i].hProcess);
            CloseHandle(jobs[i].hThread);
            // Shift remaining jobs down
            for (int j = i; j < job_count - 1; j++) {
                jobs[j] = jobs[j + 1];
                jobs[j].job_id = j + 1;
            }
            job_count--;
            return 1; // Success
        }
    }
    return 0; // Not found
}

Job* find_job(int job_id) {
    for (int i = 0; i < job_count; i++) {
        if (jobs[i].job_id == job_id) {
            return &jobs[i];
        }
    }
    return NULL;
}

void list_jobs() {
    printf("Job ID\tPID\tStatus\tCommand\n");
    for (int i = 0; i < job_count; i++) {
        printf("[%d]\t%lu\t%s\t%s\n", jobs[i].job_id, (unsigned long)jobs[i].pid,
               jobs[i].running ? "Running" : "Stopped", jobs[i].command);
    }
}

void execute_command(char **args, int background) {
    STARTUPINFO si1, si2;
    PROCESS_INFORMATION pi1, pi2;
    HANDLE hInput = NULL, hOutput = NULL;
    HANDLE hReadPipe = NULL, hWritePipe = NULL;
    char command1[1024] = "cmd.exe /c ";
    char command2[1024] = "cmd.exe /c ";
    size_t command1_length = strlen(command1);
    size_t command2_length = strlen(command2);

    // Check for pipe symbol and split args into two commands
    int i = 0;
    int pipe_index = -1;
    while (args[i] != NULL) {
        if (strcmp(args[i], "|") == 0) {
            pipe_index = i;
            break;
        }
        i++;
    }

    if (pipe_index == -1) {
        // No pipe found, proceed as before

        // Parse args for input/output redirection
        char *input_file = NULL;
        char *output_file = NULL;
        int i = 0;
        char *clean_args[100];
        int clean_index = 0;

        while (args[i] != NULL) {
            if (strcmp(args[i], "<") == 0) {
                if (args[i + 1] != NULL) {
                    input_file = args[i + 1];
                    i += 2;
                    continue;
                } else {
                    fprintf(stderr, "Error: No input file specified for redirection\n");
                    return;
                }
            } else if (strcmp(args[i], ">") == 0) {
                if (args[i + 1] != NULL) {
                    output_file = args[i + 1];
                    i += 2;
                    continue;
                } else {
                    fprintf(stderr, "Error: No output file specified for redirection\n");
                    return;
                }
            } else {
                clean_args[clean_index++] = args[i];
                i++;
            }
        }
        clean_args[clean_index] = NULL;

        // Build command string from clean_args
        for (i = 0; clean_args[i] != NULL; i++) {
            if (command1_length + strlen(clean_args[i]) + 1 < sizeof(command1)) {
                strcat(command1, clean_args[i]);
                strcat(command1, " ");
                command1_length += strlen(clean_args[i]) + 1;
            } else {
                fprintf(stderr, "Error: Command is too long to execute.\n");
                return;
            }
        }

        ZeroMemory(&si1, sizeof(si1));
        si1.cb = sizeof(si1);
        ZeroMemory(&pi1, sizeof(pi1));

        // Open input file if specified
        if (input_file != NULL) {
            hInput = CreateFile(input_file, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
            if (hInput == INVALID_HANDLE_VALUE) {
                fprintf(stderr, "Error: Unable to open input file %s\n", input_file);
                return;
            }
            si1.dwFlags |= STARTF_USESTDHANDLES;
            si1.hStdInput = hInput;
        }

        // Open output file if specified
        if (output_file != NULL) {
            hOutput = CreateFile(output_file, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
            if (hOutput == INVALID_HANDLE_VALUE) {
                fprintf(stderr, "Error: Unable to open output file %s\n", output_file);
                if (hInput != NULL) CloseHandle(hInput);
                return;
            }
            si1.dwFlags |= STARTF_USESTDHANDLES;
            si1.hStdOutput = hOutput;
            si1.hStdError = hOutput;
        }

        // Create a new process for the command
        if (!CreateProcess(NULL, command1, NULL, NULL, TRUE, background ? CREATE_NO_WINDOW : 0, NULL, NULL, &si1, &pi1)) {
            fprintf(stderr, "Error: Command execution failed for command: %s\n", command1);
            if (hInput != NULL) CloseHandle(hInput);
            if (hOutput != NULL) CloseHandle(hOutput);
            return;
        }

        if (background) {
            printf("Started background job with PID: %lu\n", (unsigned long)pi1.dwProcessId);
            add_job(pi1.hProcess, pi1.hThread, pi1.dwProcessId, command1);
        } else {
            WaitForSingleObject(pi1.hProcess, INFINITE);
        }

        // Close handles
        CloseHandle(pi1.hProcess);
        CloseHandle(pi1.hThread);
        if (hInput != NULL) CloseHandle(hInput);
        if (hOutput != NULL) CloseHandle(hOutput);

    } else {
        // Pipe found, split args into two commands
        char *cmd1_args[100];
        char *cmd2_args[100];
        int j;

        for (j = 0; j < pipe_index; j++) {
            cmd1_args[j] = args[j];
        }
        cmd1_args[pipe_index] = NULL;

        int cmd2_len = 0;
        for (j = pipe_index + 1; args[j] != NULL; j++) {
            cmd2_args[cmd2_len++] = args[j];
        }
        cmd2_args[cmd2_len] = NULL;

        // Build command strings for both commands
        for (j = 0; cmd1_args[j] != NULL; j++) {
            if (command1_length + strlen(cmd1_args[j]) + 1 < sizeof(command1)) {
                strcat(command1, cmd1_args[j]);
                strcat(command1, " ");
                command1_length += strlen(cmd1_args[j]) + 1;
            } else {
                fprintf(stderr, "Error: First command is too long to execute.\n");
                return;
            }
        }

        for (j = 0; cmd2_args[j] != NULL; j++) {
            if (command2_length + strlen(cmd2_args[j]) + 1 < sizeof(command2)) {
                strcat(command2, cmd2_args[j]);
                strcat(command2, " ");
                command2_length += strlen(cmd2_args[j]) + 1;
            } else {
                fprintf(stderr, "Error: Second command is too long to execute.\n");
                return;
            }
        }

        // Create pipe
        SECURITY_ATTRIBUTES saAttr;
        saAttr.nLength = sizeof(SECURITY_ATTRIBUTES);
        saAttr.bInheritHandle = TRUE;
        saAttr.lpSecurityDescriptor = NULL;

        if (!CreatePipe(&hReadPipe, &hWritePipe, &saAttr, 0)) {
            fprintf(stderr, "Error: Failed to create pipe.\n");
            return;
        }

        // Set up first process startup info
        ZeroMemory(&si1, sizeof(si1));
        si1.cb = sizeof(si1);
        si1.dwFlags |= STARTF_USESTDHANDLES;
        si1.hStdOutput = hWritePipe;
        si1.hStdError = hWritePipe;
        si1.hStdInput = GetStdHandle(STD_INPUT_HANDLE);

        // Set up second process startup info
        ZeroMemory(&si2, sizeof(si2));
        si2.cb = sizeof(si2);
        si2.dwFlags |= STARTF_USESTDHANDLES;
        si2.hStdInput = hReadPipe;
        si2.hStdOutput = GetStdHandle(STD_OUTPUT_HANDLE);
        si2.hStdError = GetStdHandle(STD_ERROR_HANDLE);

        // Create first process
        if (!CreateProcess(NULL, command1, NULL, NULL, TRUE, 0, NULL, NULL, &si1, &pi1)) {
            fprintf(stderr, "Error: Failed to create first process.\n");
            CloseHandle(hReadPipe);
            CloseHandle(hWritePipe);
            return;
        }

        // Create second process
        if (!CreateProcess(NULL, command2, NULL, NULL, TRUE, 0, NULL, NULL, &si2, &pi2)) {
            fprintf(stderr, "Error: Failed to create second process.\n");
            CloseHandle(hReadPipe);
            CloseHandle(hWritePipe);
            CloseHandle(pi1.hProcess);
            CloseHandle(pi1.hThread);
            return;
        }

        // Close pipe handles in parent
        CloseHandle(hWritePipe);
        CloseHandle(hReadPipe);

        if (!background) {
            WaitForSingleObject(pi1.hProcess, INFINITE);
            WaitForSingleObject(pi2.hProcess, INFINITE);
        } else {
            printf("Started background jobs with PIDs: %lu, %lu\n", (unsigned long)pi1.dwProcessId, (unsigned long)pi2.dwProcessId);
            add_job(pi1.hProcess, pi1.hThread, pi1.dwProcessId, command1);
            add_job(pi2.hProcess, pi2.hThread, pi2.dwProcessId, command2);
        }

        // Close process and thread handles
        CloseHandle(pi1.hProcess);
        CloseHandle(pi1.hThread);
        CloseHandle(pi2.hProcess);
        CloseHandle(pi2.hThread);
    }
}
#else
void execute_command(char **args, int background) {
    fprintf(stderr, "Error: Command execution is only implemented on Windows.\n");
}
#endif

void process_input(char *input) {
    char *args[MAX_ARGS];
    int background = parse_input(input, args);

    if (args[0] == NULL) {
        return; // No command entered
    }

    // Check for built-in commands first
    if (execute_builtin(args)) {
        return; // Built-in command executed
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
