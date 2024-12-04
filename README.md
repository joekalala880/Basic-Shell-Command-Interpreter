MyShell is a simple command-line interpreter written in C, similar to Unix shells like "bash".
It supports executing external commands, built-in commands (such as 'echo'), and output redirection
(">").

Features:
1. Basic Command Execution: Run standard Unix commands like 'ls', 'cat', etc.
2. Built-in 'echo' Command: Supports a custom implementation of 'echo' to print messages.
3. Output Redirection: Redirect command output to a file using '>'.
4. Process Management: Uses 'fork()' and 'execvp()' for process control, creating child processes to
execute commands.

How to Use:
Run the shell by compiling the code and running the executable:
- $ gcc myshell.c -o myshell
- $ ./myshell
The prompt ('myshell>') will appear, allowing you to enter commands. Use 'exit' to terminate the
shell.
Examples
- myshell> echo "Hello World"
- Hello World
- myshell> ls -l > output.txt
- myshell> cat output.txt

Concepts Covered:
- Command Parsing: Split commands into tokens and identify arguments and redirection operators.
- Output Redirection: Redirect standard output to files.
- Process Creation: Handle child processes using 'fork()' and replace processes using 'execvp()'.
Future Improvements
- Add support for input redirection ('<') and command piping ('|').
- Implement more built-in commands like 'cd' and 'history'.
- Support command chaining with '&&' and background execution with '&'.
