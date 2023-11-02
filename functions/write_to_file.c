#include "../morsecode.h"
#include <stddef.h>
#include <stdio.h>
#include <string.h>

/**
 * @brief writes the contents of text into the specified file.
 * 
 * @param filename the file into which the text should be written
 * @param text the text which will be written
 * 
 * @return 1 if the process failed, 0 if the process was a success
*/
int write_to_file(char* filename, volatile char* text) {
    FILE* outputFile = fopen(filename, "w");
    if (outputFile == NULL) {
        return 1;
    }

    fprintf(outputFile, "%s", text);
    fclose(outputFile);
    return 0;
}

