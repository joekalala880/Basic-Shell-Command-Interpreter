#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>

#define MAX_CMD_LEN 1024
#define MAX_ARGS 100

// Function to parse the command into arguments and check for redirection
int parse_command(char *command, char **args, char **output_file) {
    int i = 0;
    char *token = strtok(command, " ");

    while (token != NULL) {
        if (strcmp(token, ">") == 0) {
            // Output redirection
            token = strtok(NULL, " ");
            if (token != NULL) {
                *output_file = token;
            } else {
                fprintf(stderr, "Error: No output file specified for redirection\n");
                return -1;
            }
        } else {
            args[i++] = token;
        }
        token = strtok(NULL, " ");
    }
    args[i] = NULL; // Null-terminate the argument list
    return i;
}

// Function to execute a command
void execute_command(char *command) {
    char *args[MAX_ARGS];
    char *output_file = NULL;
    int arg_count;

    // Parse the command and handle redirection
    arg_count = parse_command(command, args, &output_file);

    if (arg_count == -1) {
        return; // Error in parsing, return immediately
    }

    // Debug output
    printf("Parsed arguments: ");
    for (int k = 0; k < arg_count; k++) {
        printf("%s ", args[k]);
    }
    printf("\nNumber of arguments: %d\n", arg_count);
    if (output_file) {
        printf("Redirecting output to: %s\n", output_file);
    }

    // Handle output redirection if specified
    int original_stdout = -1;
    if (output_file != NULL) {
        int fd = open(output_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
        if (fd < 0) {
            perror("Failed to open output file");
            return;
        }
        // Save the original stdout file descriptor
        original_stdout = dup(STDOUT_FILENO);
        if (dup2(fd, STDOUT_FILENO) < 0) {
            perror("Failed to redirect output");
            close(fd);
            return;
        }
        close(fd);
    }

    // Check for built-in echo command
    if (strcmp(args[0], "echo") == 0) {
        for (int k = 1; k < arg_count; k++) {
            printf("%s", args[k]); // Print argument
            if (k < arg_count - 1) {
                printf(" "); // Print space between arguments
            }
        }
        printf("\n");
        // Restore the original stdout if redirected
        if (original_stdout != -1) {
            dup2(original_stdout, STDOUT_FILENO);
            close(original_stdout);
        }
        return; // Return since it's a built-in command
    }

    // Fork and execute the command
    pid_t pid = fork();
    if (pid == 0) {
        // Child process
        execvp(args[0], args);
        perror("Command execution failed"); // Only reached if exec fails
        exit(EXIT_FAILURE);
    } else if (pid < 0) {
        perror("Fork failed");
    } else {
        // Parent process
        wait(NULL);  // Wait for child process to finish
    }

    // Restore the original stdout if redirected
    if (original_stdout != -1) {
        dup2(original_stdout, STDOUT_FILENO);
        close(original_stdout);
    }
}

// Main function to run the shell
int main() {
    char command[MAX_CMD_LEN];

    while (1) {
        printf("myshell> ");
        fflush(stdout);
        if (!fgets(command, sizeof(command), stdin)) {
            break;  // Exit on EOF
        }

        // Remove newline character
        command[strcspn(command, "\n")] = '\0';

        // Check for exit command
        if (strcmp(command, "exit") == 0) {
            break;
        }

        // Execute the command
        execute_command(command);
    }

    return 0; // Return 0 to indicate success
}
