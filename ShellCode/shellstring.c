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
 * Count the number of tokens delimited by the delimiter.
 *
 * @param const char *str: The string to check for tokens
 * @param const char *del: The character that delimits the tokens
 */
int countTokens(const char *str, const char *del) {
    int i;
    int numTokens = -1;
    char delimiter = *del;
    bool previousCharIsDelimiter = false;

    assert(str != NULL && "string cannot be NULL");
    assert(del != NULL && "cannot use a NULL delimiter");

    if (str != NULL && del != NULL) {
        numTokens = 0;
        int length = strlen(str);
        assert(length > 0);

        for (i = 0; i < length; i++) {
            if (str[i] == delimiter && previousCharIsDelimiter == false) {
                if (i != 0) {
                    numTokens++;
                }
                previousCharIsDelimiter = true;
            } else if (str[i] != delimiter) {
                previousCharIsDelimiter = false;
            }
        }

        // Because we are counting delimiters, there is possibly one token we missed
        if (previousCharIsDelimiter == false) {
            numTokens++;
        }
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
int getStringIndex(char ***strary, int strarylen, const char *searchitem) {
    int i;
    int foundIndex = -1;

    assert(strary != NULL);
    assert(strarylen > -1);
    assert(searchitem != NULL);

    for (i = 0; i < strarylen && foundIndex == -1; i++) {
        if ((*strary)[i] != NULL && strcmp((*strary)[i], searchitem) == 0) {
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
