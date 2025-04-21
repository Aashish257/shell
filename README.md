# MyShell

MyShell is a lightweight, minimalistic command-line shell implemented in C, designed to provide a simple and efficient interface for executing Unix-like commands on Windows platforms. It supports essential shell functionalities including built-in commands, background job control, piping, and input/output redirection, making it a versatile tool for basic shell operations.

## Features

- Built-in commands: `cd`, `pwd`, `exit`, `jobs` (Windows only), `kill` (Windows only), `fg` (Windows only)
- Support for background execution of commands using `&`
- Command piping to connect multiple commands
- Input and output redirection using `<` and `>`
- Basic job control for managing background processes (Windows only)
- Cross-platform compatibility with primary support for Windows environments

## Build Instructions

To compile the shell, use the following command:

```bash
gcc -o myshell main.c utils.c builtins.c
```

Ensure you have a C compiler installed (e.g., GCC) and that you are compiling on a Windows system for full functionality.

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
- This shell is intended for educational purposes and basic command execution; it does not support advanced shell scripting features.

## Contributing

Contributions are welcome! Feel free to fork the repository and submit pull requests for improvements or bug fixes.


