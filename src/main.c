#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <synchapi.h> // For WaitForSingleObject and INFINITE
#endif

#include "shell.h"
#include "history.h"
#include "jobs.h"

/**
 * Processes the user input command.
 * Parses the input, handles built-in commands, and executes external commands.
 */
void process_input(char *input) {
    char *args[MAX_ARGS];
    int background = parse_input(input, args);

    if (args[0] == NULL) {
        return;
    }

    // Add command to history if not duplicate of last command
    if (strlen(input) > 0 && (get_history_count() == 0 || strcmp(input, get_history_command(get_history_count() - 1)) != 0)) {
        add_history(input);
        save_history();
    }

    // Handle built-in job control commands
    if (strcmp(args[0], "jobs") == 0) {
        handle_jobs();
        return;
    } else if (strcmp(args[0], "kill") == 0) {
        handle_kill(args);
        return;
    } else if (strcmp(args[0], "fg") == 0) {
        handle_fg(args);
        return;
    }

    // Execute other built-in commands
    if (execute_builtin(args)) {
        return;
    }

    // Execute external command
    execute_command(args, background);
}

#ifdef _WIN32
/**
 * Reads user input with command history navigation support (Windows).
 * Supports arrow keys for navigating command history.
 */
void read_input_with_history(char *buffer, size_t size) {
    int pos = 0;
    int current_history_index = get_history_count();
    buffer[0] = '\0';

    while (1) {
        for (int key = 8; key <= 222; key++) {
            if (GetAsyncKeyState(key) & 0x8000) {
                if (key == VK_RETURN) {
                    printf("\n");
                    buffer[pos] = '\0';
                    return;
                } else if (key == VK_BACK && pos > 0) {
                    pos--;
                    buffer[pos] = '\0';
                    printf("\b \b");
                    Sleep(100);
                } else if (key == VK_UP) {
                    if (current_history_index > 0) {
                        current_history_index--;
                        for (int i = 0; i < pos; i++) {
                            printf("\b \b");
                        }
                        const char* hist_cmd = get_history_command(current_history_index);
                        if (hist_cmd) {
                            strncpy(buffer, hist_cmd, size - 1);
                            buffer[size - 1] = '\0';
                            printf("%s", buffer);
                            pos = strlen(buffer);
                        }
                    }
                    Sleep(200);
                } else if (key == VK_DOWN) {
                    if (current_history_index < get_history_count() - 1) {
                        current_history_index++;
                        for (int i = 0; i < pos; i++) {
                            printf("\b \b");
                        }
                        const char* hist_cmd = get_history_command(current_history_index);
                        if (hist_cmd) {
                            strncpy(buffer, hist_cmd, size - 1);
                            buffer[size - 1] = '\0';
                            printf("%s", buffer);
                            pos = strlen(buffer);
                        }
                    } else {
                        for (int i = 0; i < pos; i++) {
                            printf("\b \b");
                        }
                        buffer[0] = '\0';
                        pos = 0;
                    }
                    Sleep(200);
                } else if (key >= 32 && key <= 126 && pos < (int)size - 1) {
                    buffer[pos++] = (char)key;
                    buffer[pos] = '\0';
                    printf("%c", (char)key);
                    Sleep(100);
                }
                break;
            }
        }
        Sleep(10);
    }
}
#endif

/**
 * Main entry point of the shell program.
 * Loads command history, reads user input, processes commands, and saves history on exit.
 */
int main() {
    char input[MAX_INPUT_SIZE];

    load_history();

    while (1) {
        printf("> ");

#ifdef _WIN32
        read_input_with_history(input, sizeof(input));
#else
        if (fgets(input, sizeof(input), stdin) == NULL) {
            break;
        }
        input[strcspn(input, "\n")] = 0;
#endif

        process_input(input);
    }

    save_history();

    return 0;
}
