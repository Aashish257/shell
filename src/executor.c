// executor.c - Handles CreateProcess, redirection, piping

#include "executor.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>

int execute_command(char **args, int background) {
    pid_t pid;
    int status;

    pid = fork();
    if (pid < 0) {
        perror("fork failed");
        return -1;
    } else if (pid == 0) {
        // Child process
        if (execvp(args[0], args) < 0) {
            perror("execvp failed");
            exit(EXIT_FAILURE);
        }
    } else {
        // Parent process
        if (!background) {
            waitpid(pid, &status, 0);
        } else {
            printf("Process running in background with PID %d\n", pid);
        }
    }
    return 0;
}
