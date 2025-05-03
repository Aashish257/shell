#include <stdio.h>
#include <string.h>
#include "history.h"

#define HISTORY_FILE "history/command_history.txt"

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

void save_history() {
    FILE *file = fopen(HISTORY_FILE, "w");
    if (!file) return;
    for (int i = 0; i < history_count; i++) {
        fprintf(file, "%s\n", history[i]);
    }
    fclose(file);
}

void print_history() {
    for (int i = 0; i < history_count; i++) {
        printf("%d %s\n", i + 1, history[i]);
    }
}

const char* get_history_command(int index) {
    if (index < 0 || index >= history_count) {
        return NULL;
    }
    return history[index];
}

int get_history_count() {
    return history_count;
}
