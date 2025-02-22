#ifndef SHELL_H
#define SHELL_H

#define MAX_INPUT_SIZE 1024
#define MAX_ARGS 100

void handle_cd(char **args);
void handle_pwd();
void handle_exit();
void handle_help();

int execute_builtin(char **args);
int parse_input(char *input, char **args);
void execute_command(char **args, int background);

#endif
