/**
 * executor.c - Handles process creation, command execution, redirection, and piping.
 */

#include "executor.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>

/**
 * Executes a command by creating a child process.
 * @param args Array of command arguments, with args[0] being the command.
 * @param background If non-zero, runs the command in the background.
 * @return 0 on success, -1 on failure.
 */
int execute_command(char **args, int background) {
    pid_t pid;
    int status;

    pid = fork();
    if (pid < 0) {
        // Fork failed
        perror("fork failed");
        return -1;
    } else if (pid == 0) {
        // Child process: execute the command
        if (execvp(args[0], args) < 0) {
            perror("execvp failed");
            exit(EXIT_FAILURE);
        }
    } else {
        // Parent process
        if (!background) {
            // Wait for child process to finish if not background
            waitpid(pid, &status, 0);
        } else {
            // Print background process PID
            printf("Process running in background with PID %d\n", pid);
        }
    }
    return 0;
}
