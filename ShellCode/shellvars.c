/*
 * shellvars.c
 *
 * Author: Daniel Lovegrove
 *
 * Version: January 22/2018
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

    // If key doesn't exist, add it with value
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
        // If the key does exist, update the value
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
 * Try to add one new variable.
 * 
 * Precondition: initShellVarProg() has been called.
 *
 * @param char *varStatement: A variable statement like $HOME=/home
 * @param int options: Allows the possibility of suppressing stdout by passing
 *     SUPPRESS_OUTPUT. This will only suppress the success message, errors will
 *     still print. Otherwise, pass INTERACTIVE for interactivity.
 */
void tryAddVariable(char *varStatement, int options) {
    if (varStatement != NULL) {
        char *firstArg = strdup(varStatement);
        int equalsIndex = 0;
        int length = strlen(firstArg);

        if (firstArg[0] == '$') {
            while (equalsIndex < length && firstArg[equalsIndex] != '=') {
                equalsIndex++;
            }

            if (equalsIndex == length - 1) {
                printf("Cannot set an empty shell variable.\n");
            } else if (equalsIndex == length) {
                printf("Shell variables must be set with an equals character\n");
            } else {
                // This is the only case where we add the shell variable
                char *key = malloc(sizeof(char) * equalsIndex);
                char *value = malloc(sizeof(char) * (length - equalsIndex));

                // Have to manually add a null terminator here
                strncpy(key, &firstArg[1], equalsIndex - 1);
                key[equalsIndex - 1] = '\0';
                // Null terminator is copied over automatically here
                strncpy(value, &firstArg[equalsIndex + 1], length - equalsIndex);

                // Finally, add the new shell variable
                addShellVar(key, value);

                if (options != SUPPRESS_OUTPUT) {
                    printf("Variable %s updated.\n", key);
                }

                free(key);
                free(value);
            }
        } else {
            printf("Shell variables must be set starting with a $\n");
        }

        free(firstArg);
    }
}


/**
 * Search and replace shell variables in argument list. Returns true if all vars
 * were found and successfully replaced.
 *
 * Precondition: initShellVarProg() has been called.
 * 
 * @param char ***args: Array of string arguments
 * @returns bool: true if successful, false if not
 */
bool substituteShellVariables(char ***args, int arglen) {
    int i;
    bool success = true;

    for (i = 0; i < arglen && success == true; i++) {
        // Search for commands starting with $
        if ((*args)[i] != NULL && ((*args)[i])[0] == '$') {

            int length = strlen((*args)[i]);
            char *shellVarKey = malloc(length * sizeof(char));
            strncpy(shellVarKey, &((*args)[i])[1], length);

            // Try to find value associated with key
            char *shellVarValue = getShellVar(shellVarKey);

            if (shellVarValue == NULL) {
                printf("Could not find variable %s\n", shellVarKey);
                success = false;
            } else {
                // Variable is found, replace it in the argument array
                free((*args)[i]);
                (*args)[i] = strdup(shellVarValue);
            }

            free(shellVarKey);
        }
    }

    return success;
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
