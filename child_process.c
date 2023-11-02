#include "morsecode.h"
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>


/* The child functions wait indicator. */
volatile int mustWait;

/**
 * @brief after the parent process sends the SIGUSR1 signal, indicating it has finished processing the sent character the child signal handler changes mustWait to a 1, releasing the send_signals function from waiting.
 * 
 * @param sig the signal sent, SIGUSR1 in this case
 * 
 * @return void
*/
void SIGUSR1_signal_handler_child(int sig) {
    mustWait = 0;
}

/**
 * @brief sets mustWait to 1 and sends the specified signal to the parent process.
 * 
 * @param sig the signal to be sent
 * 
 * @return 0 on success, 1 otherwise
*/
int send_to_parent(int sig) {
    mustWait = 1;
    if (kill(getppid(), sig) == -1) {
        log_action("Sending signal failed.");
        printf("Sending signal failed.");
        return 1;
    }
    return 0;
}

/**
 * @brief sends signals to its parent. The signals represent morse code. The code represents the contents of the file specified. Each character is individually converted and sent. After sending a character the process waits for the parent process to send a SIGUSR1 signal back, confirming the previous character has been processed.
 * 
 * For a space SIGALRM is sent, for a '.' a SIGUSR1, for a '-' a SIGUSR2 and to indicate the end of a character a SIGINT.
 * 
 * @param fileName the file from which the message should be read, if "-" the program uses the default file.
 * 
 * @return 1 if failure or error, 0 if success
*/
int send_signals(char* fileName) {
    int inputfile;
    if (strcmp(fileName, "-") == 0) inputfile = STDIN_FILENO;
    else {
        inputfile = open(fileName, O_RDONLY);
        if (inputfile < 0) {
            log_action("Child process could not open inputfile");
            printf("Reading input file (%s) failed.\n", fileName);
            return 1;
        }
    }

    log_action("Child process started, initialising signal handler");
    signal(SIGUSR1, SIGUSR1_signal_handler_child);

    log_action("Sending signals..");
    char* buffer = malloc(4096);
    while (1) {
        int s = read(inputfile, buffer, 4096);
        if (s < 0) {
            log_action("Reading input file failed.");
            printf("Reading input file (%s) failed.", fileName);
            return -1;
        }
        if (s == 0) break;

        buffer[s] = '\0';
        int a = 0;
        int e;
        char c;
        while (buffer[a] != '\0') {
            c = buffer[a];
            if (c == ' ') e = send_to_parent(SIGALRM);
            else {
                char* morse = char_to_morse(c);
                for (int i = 0; morse[i] != '\0'; i++) {
                    if (morse[i] == '.') e = send_to_parent(SIGUSR1);
                    else if (morse[i] == '-') e = send_to_parent(SIGUSR2);
                    if (e == 1) {
                        free(buffer); 
                        return 1;
                    }
                    while (mustWait);
                }
                e = send_to_parent(SIGINT);
            }
            if (e == 1) {
                free(buffer); 
                return 1;
            }
            while (mustWait);
            a += 1;
        }
    }
    log_action("All signals sent and recieved");

    free(buffer);
    close(inputfile);
    log_action("Child process complete, ending..");
    return 0;
}

