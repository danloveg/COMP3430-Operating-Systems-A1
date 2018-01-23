/*
 * minelement.c
 *
 * Author: Daniel Lovegrove
 *
 * Version: January 23/2018
 *
 * Purpose: Find the minimum value of an array of integers using threads.
 */

#include <stdio.h>
#include <pthread.h>
#include <assert.h>
#include <string.h>


typedef struct __START_END {
    int start;
    int end;
} StartEnd;


int main (int argc, char *argv[]) {
    int i = 0;

    while (argv[i] != NULL) {
        printf("Arg %d: %s\n", i, argv[i]);
    }

}
