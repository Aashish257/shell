#ifndef SHELL_H
#define SHELL_H

#define MAX_INPUT_SIZE 1024
#define MAX_ARGS 100
#define MAX_JOBS 100
#define MAX_HISTORY 1000

#ifdef _WIN32
#include <windows.h>
#endif

void handle_cd(char **args);
void handle_pwd();
void handle_exit();
void handle_help();

#ifdef _WIN32
void handle_jobs();
void handle_kill(char **args);
void handle_fg(char **args);
#endif

int execute_builtin(char **args);
int parse_input(char *input, char **args);

// History functions and variables
void add_history(const char *command);
void load_history();
void save_history();
void print_history();
const char* get_history_command(int index);
int get_history_count();

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
