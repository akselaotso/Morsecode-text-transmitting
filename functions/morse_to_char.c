#include "../morsecode.h"

/**
 * @brief Returns character from a string of morse code.
 * 
 * The dictionary is relatively brute-force, separately defining an if statement for each variable. The if statements have been split by code length to shorten operation duration slightly, since v_strcmp, while a very efficient method of string comparison, is still O(n) time complexity. Also divided into two categories from length 3 onward, whose that start with '-' and those that do not.
 * 
 * @param morse should be the string with morse code, for example ".-" 
 * @param morseLength the length of the morse string
 * 
 * @return is the char, for ".-" this would be 'A'
*/
char morse_to_char(volatile char* morse, volatile int morseLength) {
    if (morseLength == 1) {
        if (v_strcmp(morse, ".") == 0) return 'E';
        if (v_strcmp(morse, "-") == 0) return 'T';
    } else if (morseLength == 2) {
        if (v_strcmp(morse, ".-") == 0) return 'A';
        if (v_strcmp(morse, "..") == 0) return 'I';
        if (v_strcmp(morse, "--") == 0) return 'M';
        if (v_strcmp(morse, "-.") == 0) return 'N';
    } else if (morseLength == 3) {
        if (morse[0] == '-') {
            if (v_strcmp(morse, "-..") == 0) return 'D';
            if (v_strcmp(morse, "--.") == 0) return 'G';
            if (v_strcmp(morse, "-.-") == 0) return 'K';
            if (v_strcmp(morse, "---") == 0) return 'O';
        }
        if (v_strcmp(morse, ".-.") == 0) return 'R';
        if (v_strcmp(morse, "...") == 0) return 'S';
        if (v_strcmp(morse, "..-") == 0) return 'U';
        if (v_strcmp(morse, ".--") == 0) return 'W';
    } else if (morseLength == 4) {    
        if (morse[0] == '-') {
            if (v_strcmp(morse, "-...") == 0) return 'B';
            if (v_strcmp(morse, "-.-.") == 0) return 'C';   
            if (v_strcmp(morse, "--.-") == 0) return 'Q';
            if (v_strcmp(morse, "-..-") == 0) return 'X';
            if (v_strcmp(morse, "-.--") == 0) return 'Y';
            if (v_strcmp(morse, "--..") == 0) return 'Z';
        } else {
            if (v_strcmp(morse, "..-.") == 0) return 'F';
            if (v_strcmp(morse, "....") == 0) return 'H';
            if (v_strcmp(morse, ".---") == 0) return 'J';
            if (v_strcmp(morse, ".-..") == 0) return 'L';
            if (v_strcmp(morse, ".--.") == 0) return 'P';
            if (v_strcmp(morse, "...-") == 0) return 'V';
            if (v_strcmp(morse, ".-.-") == 0) return '\n';
        }
    } else if (morseLength == 5) {
        if (morse[0] == '-') {
            if (v_strcmp(morse, "-----") == 0) return '0';  
            if (v_strcmp(morse, "-....") == 0) return '6';
            if (v_strcmp(morse, "--...") == 0) return '7';
            if (v_strcmp(morse, "---..") == 0) return '8';
            if (v_strcmp(morse, "----.") == 0) return '9'; 
            if (v_strcmp(morse, "-..-.") == 0) return '/';
            if (v_strcmp(morse, "-.--.") == 0) return '(';
            if (v_strcmp(morse, "-...-") == 0) return '=';
        } else {
            if (v_strcmp(morse, ".----") == 0) return '1';
            if (v_strcmp(morse, "..---") == 0) return '2';
            if (v_strcmp(morse, "...--") == 0) return '3';
            if (v_strcmp(morse, "....-") == 0) return '4';
            if (v_strcmp(morse, ".....") == 0) return '5';
            if (v_strcmp(morse, ".-.-.") == 0) return '+';
            if (v_strcmp(morse, ".-...") == 0) return '&';
        }
    } else if (morseLength == 6) {
        if (morse[0] == '-') {
            if (v_strcmp(morse, "-....-") == 0) return '-';
            if (v_strcmp(morse, "-.--.-") == 0) return ')';
            if (v_strcmp(morse, "---...") == 0) return ':';
            if (v_strcmp(morse, "--..--") == 0) return ',';
            if (v_strcmp(morse, "-.-.--") == 0) return '!';            
        } else {
            if (v_strcmp(morse, ".--.-.") == 0) return '@';
            if (v_strcmp(morse, ".-.-.-") == 0) return '.';
            if (v_strcmp(morse, ".-..-.") == 0) return '"';
            if (v_strcmp(morse, "..--..") == 0) return '?';
            if (v_strcmp(morse, ".----.") == 0) return '\'';
        }
    }
    return '_';
}

