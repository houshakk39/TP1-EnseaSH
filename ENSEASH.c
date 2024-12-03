#include <unistd.h>
#include <string.h>

#define BUFFER_SIZE 256

// Constants for messages
const char WELCOME_MESSAGE[] = "Welcome to the ENSEA Shell.\n";
const char QUIT_MESSAGE[] = "To exit, type 'exit'.\n";
const char PROMPT[] = "enseash % ";
const char EXIT_MESSAGE[] = "Goodbye!\n";
const char UNKNOWN_COMMAND_MESSAGE[] = "Unknown command: ";

int main() {
    char command[BUFFER_SIZE]; // Buffer to store user input
    ssize_t bytes_read;

    // Display the welcome messages
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

        // Handle unknown commands
        write(STDOUT_FILENO, UNKNOWN_COMMAND_MESSAGE, strlen(UNKNOWN_COMMAND_MESSAGE));
        write(STDOUT_FILENO, command, strnlen(command, BUFFER_SIZE));
        write(STDOUT_FILENO, "\n", 1);
    }

    // Display the exit message
    write(STDOUT_FILENO, EXIT_MESSAGE, strlen(EXIT_MESSAGE));
    return 0;
}
