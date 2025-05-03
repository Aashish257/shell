#include <stdio.h>
#include <string.h>
#include "history.h"

#define HISTORY_FILE "history/command_history.txt"

// Static array to store command history
static char history[MAX_HISTORY][MAX_INPUT_SIZE];
// Number of commands currently stored in history
static int history_count = 0;
// Current index in the history for navigation
static int history_index = 0;

/**
 * Adds a command to the history.
 * If the history is full, removes the oldest command to make space.
 */
void add_history(const char *command) {
    if (history_count < MAX_HISTORY) {
        // Copy the command to the history array
        strncpy(history[history_count], command, MAX_INPUT_SIZE - 1);
        history[history_count][MAX_INPUT_SIZE - 1] = '\0';
        history_count++;
    } else {
        // Shift history up to remove the oldest command
        memmove(history, history + 1, (MAX_HISTORY - 1) * MAX_INPUT_SIZE);
        // Add the new command at the end
        strncpy(history[MAX_HISTORY - 1], command, MAX_INPUT_SIZE - 1);
        history[MAX_HISTORY - 1][MAX_INPUT_SIZE - 1] = '\0';
    }
    // Reset history index to the end
    history_index = history_count;
}

/**
 * Loads command history from the history file.
 * Reads each line and adds it to the history array.
 */
void load_history() {
    FILE *file = fopen(HISTORY_FILE, "r");
    if (!file) return;
    char line[MAX_INPUT_SIZE];
    while (fgets(line, sizeof(line), file)) {
        // Remove newline character
        line[strcspn(line, "\n")] = 0;
        add_history(line);
    }
    fclose(file);
}

/**
 * Saves the current command history to the history file.
 */
void save_history() {
    FILE *file = fopen(HISTORY_FILE, "w");
    if (!file) return;
    for (int i = 0; i < history_count; i++) {
        fprintf(file, "%s\n", history[i]);
    }
    fclose(file);
}

/**
 * Prints the command history to standard output.
 * Each command is prefixed with its index (starting from 1).
 */
void print_history() {
    for (int i = 0; i < history_count; i++) {
        printf("%d %s\n", i + 1, history[i]);
    }
}

/**
 * Returns the command at the given index in history.
 * Returns NULL if the index is out of range.
 */
const char* get_history_command(int index) {
    if (index < 0 || index >= history_count) {
        return NULL;
    }
    return history[index];
}

/**
 * Returns the number of commands currently stored in history.
 */
int get_history_count() {
    return history_count;
}
