#include "jobs.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#ifdef _WIN32
#include <windows.h>

#define MAX_JOBS 64

typedef struct {
    HANDLE processHandle;
    DWORD processId;
    HANDLE threadHandle;
    int jobId;
    int running;
    char commandLine[256];
} Job;

static Job jobs[MAX_JOBS];
static int jobCount = 0;

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

void list_jobs() {
    printf("Background jobs:\n");
    for (int i = 0; i < jobCount; i++) {
        if (jobs[i].running) {
            printf("[%d] PID: %lu Command: %s\n", jobs[i].jobId, jobs[i].processId, jobs[i].commandLine);
        }
    }
}

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

void handle_jobs() {
    list_jobs();
}

void handle_kill(char **args) {
    if (args[1]) {
        int jobId = atoi(args[1]);
        kill_job(jobId);
    } else {
        printf("Usage: kill <jobId>\n");
    }
}

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
