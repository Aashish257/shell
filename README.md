# MyShell

MyShell is a lightweight and minimalistic shell built from scratch using C. It provides a basic command-line interface for executing Unix commands, handling processes, and more.

## Features
- Supports commands like `cd`, `pwd`, and `dir`.
- Basic command execution and input parsing.

## Installation Instructions
To compile the shell, use the following command:
```
gcc -o my_shell main.c utils.c
```

## Usage
Run the shell with:
```
./my_shell
```
You can then enter commands such as:
- `pwd`: Display the current working directory.
- `cd <directory>`: Change the current directory.
- `dir`: List the contents of the current directory.
- `exit`: Terminate the shell.
