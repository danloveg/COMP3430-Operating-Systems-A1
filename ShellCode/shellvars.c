/*
 * shellvars.c
 *
 * Author: Daniel Lovegrove
 *
 * Version: January 20/2018
 *
 * Purpose: Support adding and retrieving of shell variables.
 */


#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "shellvars.h"

#define INIT_MAX_LEN 8

typedef struct __key_value {
    char *key;
    char *value;
} KeyValObject;


int maxLength = INIT_MAX_LEN;
int currLength = 0;
KeyValObject **shellVariables = NULL;


/**
 * Create dynamic data structure and initialize operation.
 */
void initShellVarProg() {
    int i;

    assert(shellVariables == NULL && "Failure, shell variable program already initialized.");
    currLength = 0;
    maxLength = INIT_MAX_LEN;

    // Keep things simple by holding key-value objects in an array
    shellVariables = (KeyValObject**)malloc(maxLength * sizeof(KeyValObject));

    for (i = 0; i < maxLength; i++) {
        shellVariables[i] = NULL;
    }
}


/**
 * Add a new shell variable. If the variable has already been added nothing
 * happens.
 *
 * Precondition: initShellVarProg() has been called.
 *
 * @param char *key: The key to retrieve the shell variable later
 * @param char *value: The value to associate with the key
 */
void addShellVar(char *key, char *value) {
    assert(shellVariables != NULL && "Must have made prior call to initShellVarProg()");
    assert(key != NULL && value != NULL);

    char *exists = getShellVar(key);

    // Don't add if the variable is already there
    if (exists == NULL) {

        if (currLength >= maxLength) {
            // Double the size of the array
            maxLength = maxLength * 2;
            shellVariables = (KeyValObject**)realloc(shellVariables, (maxLength * sizeof(KeyValObject)));
        }

        KeyValObject *newObj = (KeyValObject *)malloc(sizeof(KeyValObject));
        assert(newObj != NULL);

        newObj -> key = strdup(key);
        newObj -> value = strdup(value);

        shellVariables[currLength++] = newObj;
    }
}


/**
 * Get an existing shell variable. Returns NULL if there is no value associated.
 * Returns a pointer to a copy of the value associated with the key for safety.
 * 
 * NOTE: The user must free the returned value.
 *
 * Precondition: initShellVarProg() has been called.
 *
 * @param char *key: The key associated with the value needed
 * @returns char *: Pointer to string value
 */
char * getShellVar(char *key) {
    int count = 0;
    char *assocValue = NULL;

    assert(shellVariables != NULL && "Must have made prior call to initShellVarProg()");
    assert(key != NULL);

    // Search for the value
    while (count < maxLength && assocValue == NULL) {

        if (shellVariables[count] != NULL && strcmp(shellVariables[count] -> key, key) == 0) {
            assocValue = strdup(shellVariables[count] -> value);
        } else if (shellVariables[count] == NULL) {
            break;
        }

        count++;
    }

    return assocValue;
}


/**
 * Free memory and clean up.
 */
void quitShellVarProg() {
    int i;

    if (shellVariables != NULL) {
        // Free all of the memory we allocated
        for (i = 0; i < maxLength; i++) {
            if (shellVariables[i] != NULL) {
                free(shellVariables[i] -> key);
                free(shellVariables[i] -> value);
                free(shellVariables[i]);
                shellVariables[i] = NULL;
            }
        }

        free(shellVariables);
        shellVariables = NULL;
        maxLength = INIT_MAX_LEN;
        currLength = 0;
    }
}
