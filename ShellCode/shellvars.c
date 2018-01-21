/*
 * shellvars.c
 *
 * Author: Daniel Lovegrove
 *
 * Version: January 20/2018
 *
 * Purpose: Support adding and retrieving of shell variables. I would have liked
 * to have used a hash table to store key-value pairs but time did not permit me
 * to implement one so I just use a List to store key-value pairs.
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
    shellVarList = malloc(sizeof(KeyValList));
    shellVarList -> currIndex = 0;
    shellVarList -> length = INIT_MAX_LEN;

    // Keep things simple by holding key-value objects in an array
    shellVarList -> shellVariables = (KeyValObject**) malloc(shellVarList -> length * sizeof(KeyValObject));

    for (i = 0; i < shellVarList -> length; i++) {
        shellVarList -> shellVariables[i] = NULL;
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
    assert(shellVarList != NULL && "Must have made prior call to initShellVarProg()");
    assert(key != NULL && value != NULL);

    char *exists = getShellVar(key);

    // Don't add if the variable is already there
    if (exists == NULL) {

        if (shellVarList -> currIndex >=  shellVarList -> length) {
            // Double the size of the array
            shellVarList -> length = shellVarList -> length * 2;
            shellVarList -> shellVariables = (KeyValObject**) realloc(shellVarList -> shellVariables, (shellVarList -> length * sizeof(KeyValObject)));
        }

        KeyValObject *newObj = (KeyValObject *)malloc(sizeof(KeyValObject));
        assert(newObj != NULL);

        newObj -> key = strdup(key);
        newObj -> value = strdup(value);

        shellVarList -> shellVariables[shellVarList -> currIndex++] = newObj;
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

    assert(shellVarList != NULL && "Must have made prior call to initShellVarProg()");
    assert(key != NULL);

    // Search for the value
    while (count < shellVarList -> length && assocValue == NULL) {

        if (shellVarList -> shellVariables[count] != NULL && strcmp(shellVarList -> shellVariables[count] -> key, key) == 0) {
            assocValue = strdup(shellVarList -> shellVariables[count] -> value);
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