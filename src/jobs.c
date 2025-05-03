#include "jobs.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#ifdef _WIN32
#include <windows.h>

#define MAX_JOBS 64

/**
 * Structure representing a background job.
 */
typedef struct {
    HANDLE processHandle;   // Handle to the process
    DWORD processId;        // Process ID
    HANDLE threadHandle;    // Handle to the thread
    int jobId;              // Job ID
    int running;            // Running status (1 = running, 0 = stopped)
    char commandLine[256];  // Command line string
} Job;

static Job jobs[MAX_JOBS];
static int jobCount = 0;

/**
 * Adds a new job to the job list.
 * @return Job ID on success, -1 if job list is full.
 */
int add_job(HANDLE processHandle, HANDLE threadHandle, DWORD processId, const char* commandLine) {
    if (jobCount >= MAX_JOBS) {
        return -1;
    }
    jobs[jobCount].processHandle = processHandle;
    jobs[jobCount].threadHandle = threadHandle;
    jobs[jobCount].processId = processId;
    jobs[jobCount].jobId = jobCount + 1;
    jobs[jobCount].running = 1;
    strncpy(jobs[jobCount].commandLine, commandLine, sizeof(jobs[jobCount].commandLine) - 1);
    jobs[jobCount].commandLine[sizeof(jobs[jobCount].commandLine) - 1] = '\0';
    jobCount++;
    return jobs[jobCount - 1].jobId;
}

/**
 * Lists all running background jobs.
 */
void list_jobs() {
    printf("Background jobs:\n");
    for (int i = 0; i < jobCount; i++) {
        if (jobs[i].running) {
            printf("[%d] PID: %lu Command: %s\n", jobs[i].jobId, jobs[i].processId, jobs[i].commandLine);
        }
    }
}

/**
 * Terminates a job by its job ID.
 * @return 0 on success, -1 on failure.
 */
int kill_job(int jobId) {
    for (int i = 0; i < jobCount; i++) {
        if (jobs[i].jobId == jobId && jobs[i].running) {
            if (TerminateProcess(jobs[i].processHandle, 1)) {
                CloseHandle(jobs[i].processHandle);
                CloseHandle(jobs[i].threadHandle);
                jobs[i].running = 0;
                printf("Job [%d] terminated.\n", jobId);
                return 0;
            } else {
                perror("Failed to terminate process");
                return -1;
            }
        }
    }
    printf("Job [%d] not found.\n", jobId);
    return -1;
}

/**
 * Brings a job to the foreground by its job ID.
 * @return 0 on success, -1 on failure.
 */
int fg_job(int jobId) {
    for (int i = 0; i < jobCount; i++) {
        if (jobs[i].jobId == jobId && jobs[i].running) {
            if (ResumeThread(jobs[i].threadHandle) != (DWORD)-1) {
                printf("Job [%d] brought to foreground.\n", jobId);
                return 0;
            } else {
                perror("Failed to resume thread");
                return -1;
            }
        }
    }
    printf("Job [%d] not found.\n", jobId);
    return -1;
}

/**
 * Handles the 'jobs' built-in command to list jobs.
 */
void handle_jobs() {
    list_jobs();
}

/**
 * Handles the 'kill' built-in command to terminate a job.
 */
void handle_kill(char **args) {
    if (args[1]) {
        int jobId = atoi(args[1]);
        kill_job(jobId);
    } else {
        printf("Usage: kill <jobId>\n");
    }
}

/**
 * Handles the 'fg' built-in command to bring a job to the foreground.
 */
void handle_fg(char **args) {
    if (args[1]) {
        int jobId = atoi(args[1]);
        fg_job(jobId);
    } else {
        printf("Usage: fg <jobId>\n");
    }
}

#else

// Stub implementations for non-Windows platforms

int add_job(void* processHandle, void* threadHandle, unsigned long processId, const char* commandLine) {
    (void)processHandle;
    (void)threadHandle;
    (void)processId;
    (void)commandLine;
    printf("Job control not supported on this platform.\n");
    return -1;
}

void list_jobs() {
    printf("Job control not supported on this platform.\n");
}

int kill_job(int jobId) {
    (void)jobId;
    printf("Job control not supported on this platform.\n");
    return -1;
}

int fg_job(int jobId) {
    (void)jobId;
    printf("Job control not supported on this platform.\n");
    return -1;
}

void handle_jobs() {
    printf("Job control not supported on this platform.\n");
}

void handle_kill(char **args) {
    (void)args;
    printf("Job control not supported on this platform.\n");
}

void handle_fg(char **args) {
    (void)args;
    printf("Job control not supported on this platform.\n");
}

#endif
