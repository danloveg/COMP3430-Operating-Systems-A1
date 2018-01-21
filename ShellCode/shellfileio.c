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

// Static file pointer
FILE *filePtr = NULL;


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
 * Precondition: OpenShellInitFile() has been called and opened file
 *     successfully.
 * 
 * @param char *str: Points to a string from the file
 */
void readFileString(char *str) {
    str = NULL;
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
