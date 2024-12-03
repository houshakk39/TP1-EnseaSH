#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>

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

// Calculate elapsed time in milliseconds
long calculate_elapsed_time(struct timespec start, struct timespec end) {
    return (end.tv_sec - start.tv_sec) * 1000 + (end.tv_nsec - start.tv_nsec) / 1000000;
}

// Display the prompt with status and execution time
void display_prompt(int status, long exec_time) {
    char prompt[BUFFER_SIZE];
    if (WIFEXITED(status)) {
        snprintf(prompt, sizeof(prompt), "enseash [exit:%d|%ldms] %% ", WEXITSTATUS(status), exec_time);
    } else if (WIFSIGNALED(status)) {
        snprintf(prompt, sizeof(prompt), "enseash [sign:%d|%ldms] %% ", WTERMSIG(status), exec_time);
    } else {
        snprintf(prompt, sizeof(prompt), "enseash %% ");
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
        // Reset interrupt flag
        interrupt_flag = 0;

        // Display the prompt
        display_prompt(status, 0);

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

        // Record the start time
        struct timespec start_time, end_time;
        clock_gettime(CLOCK_MONOTONIC, &start_time);

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

            // Record the end time
            clock_gettime(CLOCK_MONOTONIC, &end_time);

            // Calculate execution time in milliseconds
            long exec_time = calculate_elapsed_time(start_time, end_time);

            // If interrupted during execution, handle Ctrl+C
            if (interrupt_flag) {
                // If interrupted, kill the child process
                kill(pid, SIGKILL);
                status = 128 + SIGINT;
            }

            // Update prompt with execution time
            display_prompt(status, exec_time);
        }
    }

    // Display the exit message
    write(STDOUT_FILENO, EXIT_MESSAGE, strlen(EXIT_MESSAGE));
    return 0;
}
