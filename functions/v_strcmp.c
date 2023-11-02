#include "../morsecode.h"

/**
 * @brief compares a volatile string to a string
 * 
 * @param vstr the volatile char* 
 * @param str the normal char*
 * 
 * @return 0 if identical, 1 otherwise.
*/
int v_strcmp(volatile char* vstr, char* str) {    
    int i = 0;
    while (vstr[i]) {
        if (!str[i]) {
            return 1;
        } 
        if (vstr[i] != str[i]) {
            return 1;
        } 
        i += 1;
    }

    if (str[i]) {
        return 1;
    }

    return 0;
}