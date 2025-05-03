#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <synchapi.h> // For WaitForSingleObject and INFINITE
#endif

#include "shell.h"

#define HISTORY_FILE ".shell_history"

static char history[MAX_HISTORY][MAX_INPUT_SIZE];
static int history_count = 0;
static int history_index = 0;

void add_history(const char *command) {
    if (history_count < MAX_HISTORY) {
        strncpy(history[history_count], command, MAX_INPUT_SIZE - 1);
        history[history_count][MAX_INPUT_SIZE - 1] = '\0';
        history_count++;
    } else {
        memmove(history, history + 1, (MAX_HISTORY - 1) * MAX_INPUT_SIZE);
        strncpy(history[MAX_HISTORY - 1], command, MAX_INPUT_SIZE - 1);
        history[MAX_INPUT_SIZE - 1][MAX_INPUT_SIZE - 1] = '\0';
    }
    history_index = history_count;
}

void load_history() {
    FILE *file = fopen(HISTORY_FILE, "r");
    if (!file) return;
    char line[MAX_INPUT_SIZE];
    while (fgets(line, sizeof(line), file)) {
        line[strcspn(line, "\n")] = 0;
        add_history(line);
    }
    fclose(file);
}
