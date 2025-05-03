#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifdef _WIN32
#include <direct.h> // Include for _chdir function on Windows
#include <windows.h> // Include for GetCurrentDirectory on Windows
#define chdir _chdir
#define getcwd _getcwd
#define HOME_ENV "USERPROFILE"
#else
#include <unistd.h> // For chdir and getcwd on Unix-like systems
#include <limits.h> // For PATH_MAX
#ifndef PATH_MAX
#define PATH_MAX 1024
#endif
#define HOME_ENV "HOME"
#endif

#include "shell.h"
#include "jobs.h"

/**
 * Handles the 'cd' built-in command to change the current directory.
 * If no argument is provided, changes to the user's home directory.
 */
void handle_cd(char **args) {
    char *path;
    if (args[1] == NULL) {
        path = getenv(HOME_ENV);
        if (path == NULL) {
            fprintf(stderr, "cd: HOME environment variable not set\n");
            return;
        }
    } else {
        path = args[1];
    }
    if (chdir(path) != 0) {
        perror("cd failed");
    }
}

/**
 * Handles the 'pwd' built-in command to print the current working directory.
 */
void handle_pwd() {
    char cwd[PATH_MAX];
    if (getcwd(cwd, sizeof(cwd)) != NULL) {
        printf("%s\n", cwd);
    } else {
        perror("pwd failed");
    }
}

/**
 * Handles the 'exit' built-in command to exit the shell.
 */
void handle_exit() {
    printf("Exiting shell...\n");
    exit(0);
}

#ifdef _WIN32
/**
 * Handles the 'jobs' built-in command to list background jobs (Windows).
 */
void handle_jobs() {
    list_jobs();
}

/**
 * Handles the 'kill' built-in command to terminate a job by its ID (Windows).
 */
void handle_kill(char **args) {
    if (args[1] == NULL) {
        fprintf(stderr, "kill: job id required\n");
        return;
    }
    int job_id = atoi(args[1]);
    Job *job = find_job(job_id);
    if (job == NULL) {
        fprintf(stderr, "kill: job %d not found\n", job_id);
        return;
    }
    if (!TerminateProcess(job->hProcess, 1)) {
        fprintf(stderr, "kill: failed to terminate job %d\n", job_id);
    } else {
        printf("Job %d terminated\n", job_id);
        remove_job(job_id);
    }
}

/**
 * Handles the 'fg' built-in command to bring a job to the foreground (Windows).
 */
void handle_fg(char **args) {
    if (args[1] == NULL) {
        fprintf(stderr, "fg: job id required\n");
        return;
    }
    int job_id = atoi(args[1]);
    Job *job = find_job(job_id);
    if (job == NULL) {
        fprintf(stderr, "fg: job %d not found\n", job_id);
        return;
    }
    if (ResumeThread(job->hThread) == (DWORD)-1) {
        fprintf(stderr, "fg: failed to resume job %d\n", job_id);
        return;
    }
    WaitForSingleObject(job->hProcess, INFINITE);
    printf("Job %d brought to foreground and completed\n", job_id);
    remove_job(job_id);
}
#endif

/**
 * Handles the 'history' built-in command to print the command history.
 */
void handle_history() {
    print_history();
}

/**
 * Executes a built-in command if it matches one of the supported commands.
 * Returns 1 if a built-in command was executed, 0 otherwise.
 */
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
    if (strcmp(args[0], "jobs") == 0) {
        handle_jobs();
        return 1;
    }
    if (strcmp(args[0], "kill") == 0) {
        handle_kill(args);
        return 1;
    }
    if (strcmp(args[0], "fg") == 0) {
        handle_fg(args);
        return 1;
    }
    if (strcmp(args[0], "history") == 0) {
        handle_history();
        return 1;
    }
    return 0;  // Not a built-in command
}
