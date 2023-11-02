#ifndef MORSECODE_H
#define MORSECODE_H


/**
 * @brief compares a volatile string to a string
 * 
 * @file functions/v_strcmp.c
 * 
 * @param vstr the volatile char* 
 * @param str the normal char*
 * 
 * @return 0 if identical, 1 otherwise.
*/
int v_strcmp(volatile char* vstr, char* str);

/**
 * @brief writes the given text, representing the action, to the log file.
 * 
 * @file functions/log_action.c
 * 
 * @param text the text which will be written
 * 
 * @return 1 if the process failed, 0 if the process was a success
*/
int log_action(char* text);

/**
 * @brief Returns character from a string of morse code.
 * 
 * The dictionary is relatively brute-force, separately defining an if statement for each variable. The if statements have been split by code length to shorten operation duration slightly, since v_strcmp, while a very efficient method of string comparison, is still O(n) time complexity. Also divided into two categories from length 3 onward, whose that start with '-' and those that do not.
 * 
 * @file functions/morse_to_char.c
 * 
 * @param morse should be the string with morse code, for example ".-" 
 * @param morseLength the length of the morse string
 * 
 * @return is the char, for ".-" this would be 'A'
*/
char morse_to_char(volatile char* morse, volatile int morseLength);

/**
 * @brief Returns morse code for given char, converts lowercase to uppercase. 
 * 
 * @file functions/char_to_morse.c
 * 
 * @param c is the char, for example 'A'
 * 
 * @return is a string of morse, for example ".-" for 'A'
*/
char* char_to_morse(char c);

/**
 * @brief Signal handler for SIGINT. When signal is recieved converts the currentMorseCode to a character using morseToChar() and adds this to the recievedAnswer and increments recievedAnswerLength. Also resets currentMorseCode and currentMorseCodeLength. Sends child process the signal SIGUSR1 to indicate the parent process is done processing the sent signal. 
 * 
 * If recievedAnswer reallocation fails sends child termination request and changes memoryflag to 1, which will in main end the program with an error message.
 * 
 * @file main.c
 * 
 * @param sig the signal, for this handler always SIGINT
 * 
 * @return is void
*/
void SIGINT_signal_handler(int sig);

/**
 * @brief signal handler for SIGALRM. When SIGALRM is recieved, adds ' ' to recievedAnswer and increments recievedAnswerLength. Sends child process the signal SIGUSR1 to indicate the parent process is done processing the sent signal.
 * 
 * If recievedAnswer reallocation fails sends child termination request and changes memoryflag to 1, which will in main end the program with an error message.
 * 
 * @file main.c
 * 
 * @param sig the signal, for this handler always SIGALRM
 * 
 * @return is void
*/
void SIGALRM_signal_handler(int sig);

/**
 * @brief adds a '.' to currentMorseCode when SIGUSR1 is recieved and increments currentMorseCodeLength by 1. Sends child process the signal SIGUSR1 to indicate the parent process is done processing the sent signal.
 * 
 * @file main.c
 * 
 * @param sig the signal recieved - function only called when SIGUSR1 recieved
 * 
 * @return void
*/
void SIGUSR1_signal_handler(int sig);

/**
 * @brief adds a '-' to currentMorseCode when SIGUSR2 is recieved and increments currentMorseCodeLength by 1. Sends child process the signal SIGUSR1 to indicate the parent process is done processing the sent signal.
 * 
 * @file main.c
 * 
 * @param sig the signal recieved - function only called when SIGUSR2 recieved
 * 
 * @return void
*/
void SIGUSR2_signal_handler(int sig);

/**
 * @brief after the parent process sends the SIGUSR1 signal, indicating it has finished processing the sent character the child signal handler changes mustWait to a 1, releasing the send_signals function from waiting.
 * 
 * @file child_process.c
 * 
 * @param sig the signal sent, SIGUSR1 in this case
 * 
 * @return void
*/
void SIGUSR1_signal_handler_child(int sig);

/**
 * @brief sends signals to its parent. The signals represent morse code. The code represents the contents of the file specified. Each character is individually converted and sent. After sending a character the process waits for the parent process to send a SIGUSR1 signal back, confirming the previous character has been processed.
 * 
 * For a space SIGALRM is sent, for a '.' a SIGUSR1, for a '-' a SIGUSR2 and to indicate the end of a character a SIGINT.
 * 
 * @file child_process.c
 * 
 * @param fileName the file from which the message should be read
 * 
 * @return 1 if failure or error, 0 if success
*/
int send_signals(char* fileName);

/**
 * @brief sets mustWait to 1 and sends the specified signal to the parent process.
 * 
 * @file child_process.c
 * 
 * @param sig the signal to be sent
 * 
 * @return 0 on success, 1 otherwise
*/
int send_to_parent(int sig);

/**
 * @brief if input file is "-" then changes to default filename. Next forks child and runs the child program, the signal handler is triggered by the child program and compiles the received answer.
 * 
 * The child program reads the contents of the file specified and sends it back, using kill(), as morse code using SIGUSR1 for '.', SIGUSR2 for '-', SIGINT for end of character and SIGALARM for ' '.
 * 
 * @file main.c
 * 
 * @param input1 is the first input and the name of the input file
 * 
 * @return is void
*/
int fork_process(char* input1);

/**
 * @brief writes the contents of text into the specified file.
 * 
 * @param filename the file into which the text should be written
 * @param text the text which will be written
 * 
 * @file functions/write_to_file.c
 * 
 * @return 1 if the process failed, 0 if the process was a success
*/
int write_to_file(char* filename, volatile char* text);

/**
 * @brief main function. Starts by checking that argc is correct. Next initialises log file, variables (recievedAnswer and currentMorseCode) and signal handlers.
 * 
 * After initialisation starts the fork_child function. When this is done, prints result in stdout or specified file.
 * 
 * printf() writes data to stdout.
 * 
 * @file main.c
 * 
 * @param argc the number of arguments, should be 2 or more.
 * @param argv array of strings, argv[1] is the inputfile and argv[2], if specified, is the output file.
 * 
 * @return 1 if error, 0 if successfull throughout.
*/
int main(int argc, char* argv[]);

#endif

