#ifndef JOBS_H
#define JOBS_H

#ifdef _WIN32
#include <windows.h>
#endif

/**
 * Lists all running background jobs.
 */
void list_jobs();

/**
 * Terminates a job by its job ID.
 * @return 0 on success, -1 on failure.
 */
int kill_job(int jobId);

/**
 * Brings a job to the foreground by its job ID.
 * @return 0 on success, -1 on failure.
 */
int fg_job(int jobId);

/**
 * Handles the 'jobs' built-in command.
 */
void handle_jobs();

/**
 * Handles the 'kill' built-in command.
 */
void handle_kill(char **args);

/**
 * Handles the 'fg' built-in command.
 */
void handle_fg(char **args);

#endif // JOBS_H
