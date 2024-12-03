#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

#define BUFFER_SIZE 256
// CE CODE EST EXACTEMENT CELUI FOURNI EN QWUESTION 2 ENSEASH2.c OU J'AI DEJA AJOUTER LE EXIT ET CTRL+D
// Constants for messages
const char WELCOME_MESSAGE[] = "Welcome to the ENSEA Shell.\n";
const char QUIT_MESSAGE[] = "To exit, type 'exit'.\n";
const char PROMPT[] = "enseash % ";
const char EXIT_MESSAGE[] = "Goodbye!\n";

int main() {
    char command[BUFFER_SIZE]; // Buffer to store user input
    ssize_t bytes_read;

    // Display welcome messages
    write(STDOUT_FILENO, WELCOME_MESSAGE, strlen(WELCOME_MESSAGE));
    write(STDOUT_FILENO, QUIT_MESSAGE, strlen(QUIT_MESSAGE));

    // Main shell loop
    while (1) {
        // Display the prompt
        write(STDOUT_FILENO, PROMPT, strlen(PROMPT));

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
            int status;
            waitpid(pid, &status, 0);
        }
    }

    // Display the exit message
    write(STDOUT_FILENO, EXIT_MESSAGE, strlen(EXIT_MESSAGE));
    return 0;
}
