#ifndef HISTORY_H
#define HISTORY_H

/**
 * Maximum number of commands to store in history.
 */
#define MAX_HISTORY 1000

/**
 * Maximum size of each input command.
 */
#define MAX_INPUT_SIZE 1024

/**
 * Adds a command to the history.
 */
void add_history(const char *command);

/**
 * Loads command history from persistent storage.
 */
void load_history();

/**
 * Saves command history to persistent storage.
 */
void save_history();

/**
 * Prints the command history to standard output.
 */
void print_history();

/**
 * Retrieves a command from history by index.
 */
const char* get_history_command(int index);

/**
 * Returns the number of commands currently stored in history.
 */
int get_history_count();

#endif // HISTORY_H
