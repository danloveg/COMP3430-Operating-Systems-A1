/*
 * shellstring.c
 *
 * Author: Daniel Lovegrove
 *
 * Version: January 21/2018
 *
 * Purpose: Provide esoteric string operations
 */

#include <string.h>
#include <assert.h>
#include "shellstring.h"


/**
 * Count the number of tokens delimited by the delimeter.
 *
 * @param const char *str: The string to check for tokens
 * @param const char *del: The character that delimits the tokens
 */
int countTokens(const char *str, const char *del) {
    int i;
    int numTokens = -1;
    char delimiter = *del;

    assert(str != NULL && "string cannot be NULL");
    assert(del != NULL && "cannot use a NULL delimiter");

    if (str != NULL && del != NULL) {
        numTokens = 0;
        int length = strlen(str);
        assert(length > 0);

        for (i = 0; i < length; i++) {
            if (str[i] == delimiter) {
               numTokens++; 
            }
        }

        // Because we are counting delimiters, there is one token we missed
        numTokens++;
    }

    return numTokens;
}


/**
 * Get the index of a specific string in an array of strings.
 *
 * @param const char ***strary: Pointer to the array of strings to search in
 * @param int strarylen: Length of strary
 * @param const char *searchitem: The string to search for in the array
 * @returns int: The index of the string in the array. -1 if not found.
 */
int getStringIndex(const char ***strary, int strarylen, const char *searchitem) {
    int i;
    int foundIndex = -1;

    assert(strary != NULL);
    assert(strarylen > -1);
    assert(searchItem != NULL);

    for (i = 0; i < strarylen && foundIndex == -1; i++) {
        if ((*strary)[0] != NULL && strcmp((*strary)[i], searchitem) == 0) {
            foundIndex = i;
        }
    }

    return foundIndex;
}


/**
 * Remove a trailing newline character from a string.
 * 
 * @param char *str: The string
 */
void removeTrailingNewline(char *str) {
    str[strcspn(str, "\n")] = 0;
}
