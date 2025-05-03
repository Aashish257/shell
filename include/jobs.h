#ifndef JOBS_H
#define JOBS_H

#ifdef _WIN32
#include <windows.h>
#endif

void list_jobs();
int kill_job(int jobId);
int fg_job(int jobId);

void handle_jobs();
void handle_kill(char **args);
void handle_fg(char **args);

#endif // JOBS_H
