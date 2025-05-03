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

void process_input(char *input) {
    char *args[MAX_ARGS];
    int background = parse_input(input, args);

    if (args[0] == NULL) {
        return;
    }

    if (strlen(input) > 0 && (get_history_count() == 0 || strcmp(input, get_history_command(get_history_count() - 1)) != 0)) {
        add_history(input);
        save_history();
    }

    if (execute_builtin(args)) {
        return;
    }

    execute_command(args, background);
}

#ifdef _WIN32
void read_input_with_history(char *buffer, size_t size) {
    int pos = 0;
    int current_history_index = history_count;
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
                        strncpy(buffer, get_history_command(current_history_index), size - 1);
                        buffer[size - 1] = '\0';
                        printf("%s", buffer);
                        pos = strlen(buffer);
                    }
                    Sleep(200);
                } else if (key == VK_DOWN) {
                    if (current_history_index < history_count - 1) {
                        current_history_index++;
                        for (int i = 0; i < pos; i++) {
                            printf("\b \b");
                        }
                        strncpy(buffer, get_history_command(current_history_index), size - 1);
                        buffer[size - 1] = '\0';
                        printf("%s", buffer);
                        pos = strlen(buffer);
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
