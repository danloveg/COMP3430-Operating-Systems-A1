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
#include <sys/types.h>
#include <pwd.h>
#include "shellfileio.h"


/**
 * Tries to open a shell initialization file called .shell_init in the user's
 * home directory.
 * 
 * @returns int: 0 if file was found and opened, 1 if file not found/error
 */
int openShellInitFile() {
    int fileOpened = 1;

    char *userHomeDirectory = getenv("HOME");

    if (userHomeDirectory == NULL) {
        userHomeDirectory = getpwuid(getuid()) -> pw_dir;
    }

    return fileOpened;
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
    return 1;
}
