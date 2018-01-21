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
 * Remove a trailing newline character from a string.
 * 
 * @param char *str: The string
 */
void removeTrailingNewline(char *str) {
    str[strcspn(str, "\n")] = 0;
}
