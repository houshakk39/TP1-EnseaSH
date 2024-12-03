#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <stdio.h>

#define BUFFER_SIZE 256

// Constants for messages
const char WELCOME_MESSAGE[] = "Welcome to the ENSEA Shell.\n";
const char QUIT_MESSAGE[] = "To exit, type 'exit'.\n";
const char EXIT_MESSAGE[] = "Goodbye!\n";

// Global variables for handling Ctrl+C
volatile sig_atomic_t interrupt_flag = 0;

// Signal handler for SIGINT (Ctrl+C)
void handle_sigint(int sig) {
    (void)sig; // Suppress unused parameter warning
    interrupt_flag = 1;
}

void display_prompt(int status) {
    char prompt[BUFFER_SIZE];
    if (WIFEXITED(status)) {
        // Normal exit, get exit code
        snprintf(prompt, sizeof(prompt), "enseash [exit:%d] %% ", WEXITSTATUS(status));
    } else if (WIFSIGNALED(status)) {
        // Abnormal exit, get signal number
        snprintf(prompt, sizeof(prompt), "enseash [sign:%d] %% ", WTERMSIG(status));
    } else {
        // Default prompt
        strncpy(prompt, "enseash % ", sizeof(prompt));
    }
    write(STDOUT_FILENO, prompt, strlen(prompt));
}

int main() {
    char command[BUFFER_SIZE]; // Buffer to store user input
    ssize_t bytes_read;
    int status = 0;

    // Register SIGINT handler
    signal(SIGINT, handle_sigint);

    // Display welcome messages
    write(STDOUT_FILENO, WELCOME_MESSAGE, strlen(WELCOME_MESSAGE));
    write(STDOUT_FILENO, QUIT_MESSAGE, strlen(QUIT_MESSAGE));

    // Main shell loop
    while (1) {
        // Display the prompt
        display_prompt(status);

        // Reset interrupt flag
        interrupt_flag = 0;

        // Read user input
        bytes_read = read(STDIN_FILENO, command, BUFFER_SIZE - 1);
        if (bytes_read <= 0) {
            break; // Exit on EOF (Ctrl+D) or error
        }

        // Null-terminate the input string and remove the newline character
        command[bytes_read - 1] = '\0';

        // Check if the user wants to exit
        if (strncmp(command, "exit", BUFFER_SIZE) == 0) {
            break; // Exit the loop
        }

        // If interrupted by Ctrl+C, skip execution
        if (interrupt_flag) {
            write(STDOUT_FILENO, "\n", 1);
            status = 128 + SIGINT; // Set status to signal interrupt
            continue;
        }

        // Fork a child process to execute the command
        pid_t pid = fork();
        if (pid == -1) {
            // Error in forking
            const char error_msg[] = "Error: Unable to create process.\n";
            write(STDOUT_FILENO, error_msg, strlen(error_msg));
            continue;
        }

        if (pid == 0) {
            // Child process
            // Execute the command
            execlp(command, command, NULL);
            // If execlp fails, print an error message and exit
            const char exec_error_msg[] = "Error: Command not found.\n";
            write(STDOUT_FILENO, exec_error_msg, strlen(exec_error_msg));
            _exit(1);
        } else {
            // Parent process
            // Wait for the child process to complete
            waitpid(pid, &status, 0);

            // Check for Ctrl+C while waiting
            if (interrupt_flag) {
                // If interrupted, kill the child process
                kill(pid, SIGKILL);
                status = 128 + SIGINT;
            }
        }
    }

    // Display the exit message
    write(STDOUT_FILENO, EXIT_MESSAGE, strlen(EXIT_MESSAGE));
    return 0;
}
