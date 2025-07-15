# 🐚 MyShell

> A lightweight, minimalistic command-line shell implemented in **C**, designed to execute Unix-like commands on **Windows and Unix-like platforms**.  
Supports essential shell functionalities including built-in commands, background jobs, piping, and I/O redirection.

![License](https://img.shields.io/badge/license-MIT-blue.svg)
![Platform](https://img.shields.io/badge/platform-Windows%20%7C%20Unix-lightgrey)
![Language](https://img.shields.io/badge/language-C-brightgreen)

---

## 🚀 Features

✅ **Built-in commands**: `cd`, `pwd`, `exit`, `history`, and on Windows: `jobs`, `kill`, `fg`  
✅ **Background execution** using `&`  
✅ **Command piping** (`|`) to chain commands  
✅ **Input & output redirection** (`<`, `>`)  
✅ **Basic job control** (Windows only)  
✅ **Persistent command history** (`history/command_history.txt`)  
✅ **Cross-platform support**, primarily Windows

---

## 📂 Table of Contents
- [Features](#-features)
- [Source Structure](#-source-structure)
- [Build Instructions](#-build-instructions)
- [Usage](#-usage)
- [Examples](#-examples)
- [Notes](#-notes)
- [Contributing](#-contributing)
- [License](#-license)
- [Contact](#-contact)

---

## 🗂 Source Structure

| File                  | Description                                  |
|------------------------|---------------------------------------------|
| `src/main.c`            | Main program & user input loop              |
| `src/utils.c`           | Helper utility functions                   |
| `src/builtins.c`        | Built-in shell commands implementation     |
| `src/executor.c`        | Command execution & process management     |
| `src/jobs.c`            | Background job control (Windows only)      |
| `src/history.c`         | Command history management                 |
| `src/parser.c`          | Placeholder for advanced parsing           |
| `include/utils.h`       | Utility function declarations              |

---

## 🛠 Build Instructions

Make sure you have GCC installed.

```bash
gcc -o myshell src/main.c src/utils.c src/builtins.c src/executor.c src/jobs.c src/history.c
```

- ✅ Recommended to compile & run on **Windows** for full job control functionality.
- On Unix-like systems, it works for general commands but without `jobs`, `fg`, and `kill`.

---

## 💻 Usage

Run your shell executable:

```bash
./myshell
```

---

## ⚙ Examples

- **Basic commands**
  ```bash
  pwd
  cd <directory>
  dir
  exit
  history
  ```

- **Run in background**
  ```bash
  notepad &
  ```

- **Piping**
  ```bash
  dir | find "txt"
  ```

- **Redirection**
  ```bash
  dir > output.txt
  type < input.txt
  ```

- **Job control (Windows only)**
  ```bash
  jobs
  kill <job_id>
  fg <job_id>
  ```

---

## 📝 Notes

- On Windows, `pwd` might not be recognized — use `echo %cd%`.
- `jobs`, `kill`, `fg` only work on **Windows**.
- Command history is saved at `history/command_history.txt`.
- Parser is currently a placeholder; does **not support complex syntax or scripting**.
- Intended primarily for educational purposes & simple shell operations.

---

## 🤝 Contributing

Contributions are welcome!  
- Fork the repository
- Create a feature branch
- Submit a pull request

---

## ⚖ License

This project is licensed under the **MIT License**.

---

## 📧 Contact

For queries or suggestions, feel free to reach out or open an issue.
