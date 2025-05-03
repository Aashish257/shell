#ifndef HISTORY_H
#define HISTORY_H

#define MAX_HISTORY 1000
#define MAX_INPUT_SIZE 1024

void add_history(const char *command);
void load_history();
void save_history();
void print_history();
const char* get_history_command(int index);
int get_history_count();

#endif // HISTORY_H
