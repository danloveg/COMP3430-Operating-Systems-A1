/*
 * shellfileio.c
 *
 * Author: Daniel Lovegrove
 *
 * Version: January 21/2018
 *
 * Purpose: Support reading from a shell intialization file.
 */

#include <stdio.h>
#include <assert.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <pwd.h>
#include "shellfileio.h"


FILE *filePtr = NULL;
char charBuf[256];
char *characterBuffer = &charBuf[0];


/**
 * Tries to open a shell initialization file called .shell_init in the user's
 * home directory.
 * 
 * @returns int: 0 if file was found and opened, 1 if file not found/error
 */
int openShellInitFile() {
    int openSuccess = 1;

    char *userHomeDirectory = getenv("HOME");

    if (userHomeDirectory == NULL) {
        userHomeDirectory = getpwuid(getuid()) -> pw_dir;
    }

    // Construct the absolute path to the shell init file
    int homeDirLength = strlen(userHomeDirectory);
    int shellInitLength = strlen(SHELL_INIT_FILE_NAME);
    int absolutePathLength = homeDirLength + shellInitLength;

    char *filename = malloc((absolutePathLength + 2) * sizeof(char));
    assert(filename != NULL);
    strcpy(filename, userHomeDirectory);
    filename[homeDirLength] = '/';
    strcpy(&filename[homeDirLength + 1], SHELL_INIT_FILE_NAME);
    filename[absolutePathLength + 1] = '\0';

    // Check to see if we can open the file
    if (access(filename, F_OK) != -1) {
        filePtr = fopen(filename, "r");
        if (filePtr != NULL) {
            openSuccess = 0;
        }
    } else {
        printf("Error: Could not open %s\n", filename);
    }

    free(filename);
    return openSuccess;
}


/**
 * Gets one line from the open file.
 * 
 * Note: str points to a dynamically created string, so it must be freed later.
 *
 * Precondition: openShellInitFile() has been called and opened file
 *     successfully.
 *
 * @returns char *: Pointer to dynamically created string
 */
char * readFileString() {
    // Read a character at a time, return a full string
    assert(filePtr != NULL && "File must be open to read");

    char *str = NULL;

    int lineLength = 0;
    char c = fgetc(filePtr);

    while (c != EOF && c != '\n') {
        characterBuffer[lineLength++] = c;
        c = fgetc(filePtr);
    }

    // Put a null terminator at the end to make it a string
    characterBuffer[lineLength] = '\0';

    if (lineLength != 0 && c != EOF) {
        str = strdup(characterBuffer);
    }

    return str;
}


/**
 * Closes the shell intialization file.
 * 
 * Precondition: OpenShellInitFile() has been called and opened file
 *     successfully.
 * 
 * @returns int: 0 if file closed, 1 if there was an error.
 */
int closeShellInitFile() {
    int closeSuccess = 0;

    if (filePtr != NULL) {
        fclose(filePtr);
    }

    return closeSuccess;
}
