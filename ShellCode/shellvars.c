/*
 * shellvars.c
 *
 * Author: Daniel Lovegrove
 *
 * Version: January 20/2018
 *
 * Purpose: Support adding and retrieving of shell variables. Stores key-value
 * pairs in a List (rather than the more typical hash table).
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

typedef struct __key_value_list {
    int length;
    int currIndex;
    KeyValObject **shellVariables;
} KeyValList;


// List struct that holds shell variables
KeyValList *shellVarList = NULL;


/**
 * Create dynamic data structure and initialize operation.
 */
void initShellVarProg() {
    int i;

    assert(shellVarList == NULL && "Failure, shell variable program already initialized.");

    // Dynamically allocate a list to store shell variables
    shellVarList = (KeyValList*) malloc(sizeof(KeyValList));
    assert(shellVarList != NULL);
    shellVarList -> currIndex = 0;
    shellVarList -> length = INIT_MAX_LEN;
    shellVarList -> shellVariables = (KeyValObject**) malloc(shellVarList -> length * sizeof(KeyValObject));
    assert(shellVarList -> shellVariables != NULL);

    for (i = 0; i < shellVarList -> length; i++) {
        shellVarList -> shellVariables[i] = NULL;
    }
}


/**
 * Add a new shell variable. If the variable has already been added then it is
 * updated.
 *
 * Precondition: initShellVarProg() has been called.
 *
 * @param char *key: The key to retrieve the shell variable later
 * @param char *value: The value to associate with the key
 */
void addShellVar(char *key, char *value) {
    assert(shellVarList != NULL && "Must have made prior call to initShellVarProg()");
    assert(key != NULL && value != NULL && "Cannot pass NULL pointers as args");

    char *exists = getShellVar(key);

    // Don't add if the variable is already there
    if (exists == NULL) {

        // Double the size of the array if need be
        if (shellVarList -> currIndex >=  shellVarList -> length) {
            shellVarList -> length = shellVarList -> length * 2;
            shellVarList -> shellVariables = (KeyValObject**) realloc(shellVarList -> shellVariables, (shellVarList -> length * sizeof(KeyValObject)));
            assert(shellVarList -> shellVariables != NULL);
        }

        KeyValObject *newObj = (KeyValObject *)malloc(sizeof(KeyValObject));
        assert(newObj != NULL);

        newObj -> key = strdup(key);
        newObj -> value = strdup(value);
        assert(newObj -> key != NULL && newObj -> value != NULL);

        shellVarList -> shellVariables[shellVarList -> currIndex++] = newObj;
    } else {
        // Find the index of the key
        int i, found = 0;

        while (found == 0 && i < shellVarList -> currIndex) {
            if (strcmp(key, shellVarList -> shellVariables[i] -> key) == 0) {
                found = 1;
            } else {
                i++;
            }
        }

        // free the current shellVariable value
        free(shellVarList -> shellVariables[i] -> value);
        shellVarList -> shellVariables[i] -> value = strdup(value);
    }

    free(exists);
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

    assert(shellVarList != NULL && "Must have made prior call to initShellVarProg()");
    assert(key != NULL && "cannot pass a NULL key");

    // Search for the value
    while (count < shellVarList -> length && assocValue == NULL) {

        if (shellVarList -> shellVariables[count] != NULL && strcmp(shellVarList -> shellVariables[count] -> key, key) == 0) {
            assocValue = strdup(shellVarList -> shellVariables[count] -> value);
            assert(assocValue != NULL);
        } else if (shellVarList -> shellVariables[count] == NULL) {
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

    if (shellVarList != NULL && shellVarList -> shellVariables != NULL) {
        // Free all of the memory we allocated
        for (i = 0; i < shellVarList -> length; i++) {
            if (shellVarList -> shellVariables[i] != NULL) {
                free(shellVarList -> shellVariables[i] -> key);
                free(shellVarList -> shellVariables[i] -> value);
                free(shellVarList -> shellVariables[i]);
                shellVarList -> shellVariables[i] = NULL;
            }
        }

        free(shellVarList -> shellVariables);
        shellVarList -> shellVariables = NULL;
        free(shellVarList);
        shellVarList = NULL;
    }
}
