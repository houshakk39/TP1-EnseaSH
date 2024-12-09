#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_SIZE 256

// Define your messages
const char HELLO[] = "Welcome to the ENSEA Shell.\n";
const char REGULAR_PROMPT[] = "enseash % ";
const char BYE[] = "Goodbye!\n";
const char ERR[] = "Error: Command not found.\n";
const char EXIT[] = "exit";

const char SIGNALED_PROMPT[] = "[sign:";
const char EXITED_PROMPT[] = "[exit:";
const char END_PROMPT[] = "|";
const char NEWLINE[] = "]\n";

int main(void) {
    char commande[MAX_SIZE] = {0};  // Buffer to store the user input
    char display[MAX_SIZE] = {0};   // Buffer to display the status message

    int commande_size;
    int pid;
    int status;
    int test;

    struct timespec timestart;
    struct timespec timestop;

    // Display the welcome message and the initial prompt
    write(STDOUT_FILENO, HELLO, strlen(HELLO));
    write(STDOUT_FILENO, REGULAR_PROMPT, strlen(REGULAR_PROMPT));

    while (1) {
        commande_size = read(STDIN_FILENO, commande, MAX_SIZE);

        if (commande_size == 0) {
            // EOF received (Ctrl+D)
            write(STDOUT_FILENO, BYE, strlen(BYE));
            kill(getpid(), SIGINT);
        }

        if (commande_size < 0) {
            perror("read");
            exit(EXIT_FAILURE);
        }

        // Remove the trailing newline character
        if (commande[commande_size - 1] == '\n') {
            commande[commande_size - 1] = '\0';
        }

        // Check if the user typed "exit"
        test = strcmp(EXIT, commande);
        if (test == 0) {
            write(STDOUT_FILENO, BYE, strlen(BYE));
            kill(getpid(), SIGINT);
        }

        // Record the start time before executing the command
        clock_gettime(CLOCK_REALTIME, &timestart);

        pid = fork();

        if (pid == -1) {
            write(STDOUT_FILENO, ERR, strlen(ERR));
            continue;
        }

        if (pid == 0) {
            // Child process: attempt to execute the command
            execlp(commande, commande, NULL);

            // If execlp fails
            write(STDOUT_FILENO, ERR, strlen(ERR));
            _exit(1);
        } else {
            // Parent process: wait for the child process to complete
            wait(&status);

            // Record the stop time after the command finishes
            clock_gettime(CLOCK_REALTIME, &timestop);

            long time = (timestop.tv_sec - timestart.tv_sec) * 1000 +
                         (timestop.tv_nsec - timestart.tv_nsec) / 1000000;

            if (WIFSIGNALED(status)) {
                // Child process terminated by a signal
                char message[MAX_SIZE];
                snprintf(message, MAX_SIZE, "%s%d%s%ld%s%s", SIGNALED_PROMPT, WTERMSIG(status), END_PROMPT, time, "ms", NEWLINE);
                write(STDOUT_FILENO, message, strlen(message));
            } else if (WIFEXITED(status)) {
                // Child process exited normally
                char message[MAX_SIZE];
                snprintf(message, MAX_SIZE, "%s%d%s%ld%s%s", EXITED_PROMPT, WEXITSTATUS(status), END_PROMPT, time, "ms", NEWLINE);
                write(STDOUT_FILENO, message, strlen(message));
            }

            // Re-display the prompt
            write(STDOUT_FILENO, REGULAR_PROMPT, strlen(REGULAR_PROMPT));
        }
    }

    return 0;
}
