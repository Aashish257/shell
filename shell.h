#ifndef SHELL_H
#define SHELL_H

#define MAX_INPUT_SIZE 1024
#define MAX_ARGS 100
#define MAX_JOBS 100

#ifdef _WIN32
#include <windows.h>
#endif

void handle_cd(char **args);
void handle_pwd();
void handle_exit();
void handle_help();

int execute_builtin(char **args);
int parse_input(char *input, char **args);

#ifdef _WIN32

typedef struct {
    int job_id;
    HANDLE hProcess;
    HANDLE hThread;
    DWORD pid;
    char command[MAX_INPUT_SIZE];
    int running; // 1 if running, 0 if finished
} Job;

extern Job jobs[MAX_JOBS];
extern int job_count;

void add_job(HANDLE hProcess, HANDLE hThread, DWORD pid, const char *command);
int remove_job(int job_id);
Job* find_job(int job_id);
void list_jobs();

#endif

#endif
