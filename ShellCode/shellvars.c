/*
 * shellvars.c
 *
 * Author: Daniel Lovegrove
 *
 * Version: January 20/2018
 *
 * Purpose: Support adding and retrieving of shell variables.
 */


#include "shellvars.h"


/**
 * Create dynamic data structure and initialize operation.
 */
void initShellVarProg() {

}


/**
 * Add a new shell variable.
 *
 * Precondition: initShellVarProg() has been called.
 *
 * @param char *key: The key to retrieve the shell variable later
 * @param char *value: The value to associate with the key
 */
void addShellVar(char *key, char *value) {

}


/**
 * Get an existing shell variable. Returns NULL if there is no key. Returns a
 * pointer to a copy of the value associated with the key for safety.
 *
 * Precondition: initShellVarProg() has been called.
 *
 * @param char *key: The key associated with the value needed
 * @returns char *: Pointer to string value
 */
char * getShellVar(char *key) {
    // I shouldn't return a pointer directly to a string in my array, I should
    // create a copy and return a pointer to it.
}


/**
 * Free memory and clean up.
 */
void quitShellVarProg() {

}

