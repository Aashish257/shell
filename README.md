# MyShell

MyShell is a lightweight, minimalistic command-line shell implemented in C, designed to provide a simple and efficient interface for executing Unix-like commands on Windows and Unix-like platforms. It supports essential shell functionalities including built-in commands, background job control, piping, and input/output redirection, making it a versatile tool for basic shell operations.

## Features

- Built-in commands: `cd`, `pwd`, `exit`, `jobs` (Windows only), `kill` (Windows only), `fg` (Windows only), `history`
- Support for background execution of commands using `&`
- Command piping to connect multiple commands
- Input and output redirection using `<` and `>`
- Basic job control for managing background processes (Windows only)
- Command history with persistent storage in `history/command_history.txt`
- Cross-platform compatibility with primary support for Windows environments
- Utility helper functions declared in `include/utils.h`
- Parser implementation is currently a placeholder

## Source Files

- `src/main.c`: Main program and input processing
- `src/utils.c`: Utility functions including input parsing
- `src/builtins.c`: Built-in shell commands implementation
- `src/executor.c`: Command execution and process management
- `src/jobs.c`: Job control implementation (Windows only)
- `src/history.c`: Command history management
- `src/parser.c`: Command parser (to be implemented)

## Build Instructions

To compile the shell, use the following command:

```bash
gcc -o myshell src/main.c src/utils.c src/builtins.c src/executor.c src/jobs.c src/history.c
```

Ensure you have a C compiler installed (e.g., GCC). For full job control functionality, compile and run on a Windows system.

## Usage

Run the shell executable:

```bash
./myshell
```

You can then enter commands such as:

- `pwd`: Display the current working directory.
- `cd <directory>`: Change the current directory.
- `dir`: List the contents of the current directory (external command).
- `exit`: Terminate the shell.
- `jobs`: List background jobs (Windows only).
- `kill <job_id>`: Terminate a background job by its job ID (Windows only).
- `fg <job_id>`: Bring a background job to the foreground (Windows only).
- `history`: Display the command history.
- Use `&` at the end of a command to run it in the background, e.g., `notepad &`.

You can also use piping and redirection, for example:

```bash
dir | find "txt"
dir > output.txt
type < input.txt
```

## Additional Notes

- The `pwd` command may not be recognized in Windows Command Prompt; use `echo %cd%` instead to display the current directory.
- Job control features (`jobs`, `kill`, `fg`) are only available on Windows platforms.
- Command history is saved persistently in `history/command_history.txt`.
- The parser implementation is currently a placeholder and may not support complex command parsing.
- This shell is intended for educational purposes and basic command execution; it does not support advanced shell scripting features.

## Contributing

Contributions are welcome! Feel free to fork the repository and submit pull requests for improvements or bug fixes.
