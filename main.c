#include "morsecode.h"
#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include <sys/types.h>
#include <string.h>
#include <sys/wait.h>

/* Longest morse code is 8, certainly small enough for static allocation. */
volatile char currentMorseCode[10];
volatile int currentMorseCodeLength = 0;

/* The recievedAnswer string is dynamically allocated. */ 
volatile char* recievedAnswer;
volatile int recievedAnswerLength = 0;

volatile pid_t childPID;
volatile int memoryErrorFlag = 0;

/**
 * @brief Signal handler for SIGINT. When signal is recieved converts the currentMorseCode to a character using morseToChar() and adds this to the recievedAnswer and increments recievedAnswerLength. Also resets currentMorseCode and currentMorseCodeLength. Sends child process the signal SIGUSR1 to indicate the parent process is done processing the sent signal. 
 * 
 * If recievedAnswer reallocation fails sends child termination request and changes memoryflag to 1, which will in main end the program with an error message.
 * 
 * @param sig the signal, for this handler always SIGINT
 * 
 * @return is void
*/
void SIGINT_signal_handler(int sig) {
    recievedAnswer = (char*)realloc((void*)recievedAnswer, (recievedAnswerLength + 2) * sizeof(char));
    if (recievedAnswer == NULL) {
        memoryErrorFlag = 1;
        log_action("Parent encountered realloc error while processing SIGINT, sending SIGTERM to child..");
        kill(childPID, SIGTERM);
    } else {
        if (morse_to_char(currentMorseCode, currentMorseCodeLength) == '_') {kill(childPID, SIGUSR1); return;} /* Ignore \r (or other unknown char)*/
        recievedAnswer[recievedAnswerLength] = morse_to_char(currentMorseCode, currentMorseCodeLength);
        recievedAnswer[recievedAnswerLength + 1] = '\0';
        currentMorseCode[0] = '\0';
        currentMorseCodeLength = 0;
        recievedAnswerLength += 1;
        kill(childPID, SIGUSR1);
    }
}

/**
 * @brief signal handler for SIGALRM. When SIGALRM is recieved, adds ' ' to recievedAnswer and increments recievedAnswerLength. Sends child process the signal SIGUSR1 to indicate the parent process is done processing the sent signal.
 * 
 * If recievedAnswer reallocation fails sends child termination request and changes memoryflag to 1, which will in main end the program with an error message.
 * 
 * @param sig the signal, for this handler always SIGALRM
 * 
 * @return is void
*/
void SIGALRM_signal_handler(int sig) {
    recievedAnswer = (char*)realloc((void*)recievedAnswer, (recievedAnswerLength + 2) * sizeof(char));
    if (recievedAnswer == NULL) {
        memoryErrorFlag = 1;
        log_action("Parent encountered realloc error while processing SIGALRM, sending SIGTERM to child..");
        kill(childPID, SIGTERM);
    } else {
        recievedAnswer[recievedAnswerLength] = ' ';
        recievedAnswer[recievedAnswerLength + 1] = '\0';
        recievedAnswerLength += 1;
        kill(childPID, SIGUSR1);
    }
}

/**
 * @brief adds a '.' to currentMorseCode when SIGUSR1 is recieved and increments currentMorseCodeLength by 1. Sends child process the signal SIGUSR1 to indicate the parent process is done processing the sent signal.
 * 
 * @param sig the signal recieved - function only called when SIGUSR1 recieved
 * 
 * @return void
*/
void SIGUSR1_signal_handler(int sig) {
    currentMorseCode[currentMorseCodeLength] = '.';
    currentMorseCode[currentMorseCodeLength + 1] = '\0';
    currentMorseCodeLength += 1;
    kill(childPID, SIGUSR1);
}

/**
 * @brief adds a '-' to currentMorseCode when SIGUSR2 is recieved and increments currentMorseCodeLength by 1. Sends child process the signal SIGUSR1 to indicate the parent process is done processing the sent signal.
 * 
 * @param sig the signal recieved - function only called when SIGUSR2 recieved
 * 
 * @return void
*/
void SIGUSR2_signal_handler(int sig) {
    currentMorseCode[currentMorseCodeLength] = '-';
    currentMorseCode[currentMorseCodeLength + 1] = '\0';
    currentMorseCodeLength += 1;
    kill(childPID, SIGUSR1);
}

/**
 * @brief if input file is "-" then changes to default filename. Next forks child and runs the child program, the signal handler is triggered by the child program and compiles the received answer.
 * 
 * The child program reads the contents of the file specified and sends it back, using kill(), as morse code using SIGUSR1 for '.', SIGUSR2 for '-', SIGINT for end of character and SIGALARM for ' '.
 * 
 * @param input1 is the first input and the name of the input file
 * 
 * @return is void
*/
int fork_process(char* input1) {
    if ((childPID = fork()) < 0) {
        perror("Fork failed.\n");
        return 1;
    } else if (childPID == 0) {
        if (send_signals(input1) != 0) {
            log_action("Child process failed to send all signals");
            printf("Sending signals failed.\n");
            return 1;
        }
        exit(1);
    } else {
        wait(NULL);
        log_action("Parent process proceeding after fork..");
    }
    return 0;
}

/**
 * @brief main function. Starts by checking that argc is correct. Next initialises log file, variables (recievedAnswer and currentMorseCode) and signal handlers.
 * 
 * After initialisation starts the fork_child function. When this is done, prints result in stdout or specified file.
 * 
 * printf() writes data to stdout.
 * 
 * @param argc the number of arguments, should be 2 or more.
 * @param argv array of strings, argv[1] is the inputfile and argv[2], if specified, is the output file.
 * 
 * @return 1 if error, 0 if success.
*/
int main(int argc, char* argv[]) {
    if (argc < 2) {
        printf("Usage: %s <inputfile or -> <outputfile, - or empty>\n", argv[0]);
        return 1;
    }

    FILE* logfile = fopen("log_file.txt", "w");
    if (logfile == NULL) {
        printf("Error creating logfile. \n");
        return 1;
    }
    fclose(logfile);

    log_action("Log file created");
    log_action("Initialising parent variables and signal handlers");
    recievedAnswer = (volatile char*)malloc(1 * sizeof(char));
    recievedAnswer[0] = '\0';
    currentMorseCode[0] = '\0';
    
    signal(SIGUSR1, SIGUSR1_signal_handler);
    signal(SIGUSR2, SIGUSR2_signal_handler);
    signal(SIGALRM, SIGALRM_signal_handler);
    signal(SIGINT, SIGINT_signal_handler);


    log_action("Starting fork");
    if (fork_process(argv[1]) != 0) {
        log_action("Fork failed. ");
        printf("Fork failed.");
        free((void*)recievedAnswer);
        return 1;
    }

    log_action("Checking memory allocation failure.");
    if (memoryErrorFlag != 0) {
        log_action("Memory allocation failed");
        printf("Memory allocation failed.");
        free((void*)recievedAnswer);
        return 1;
    }

    log_action("Outputting message to stdout or specified file");
    if (argc == 2) printf("%s", recievedAnswer);
    else if (strcmp(argv[2], "-") == 0) printf("%s", recievedAnswer);
    else {
        if (write_to_file(argv[2], recievedAnswer) != 0) {
            log_action("Writing to output file failed");
            printf("Writing to file %s failed.\n", argv[2]);
            return 1;
        } 
    }

    free((void*)recievedAnswer);
    log_action("Ending program");
    return 0;
}

