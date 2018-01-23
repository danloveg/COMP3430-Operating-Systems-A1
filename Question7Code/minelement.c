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
#include <stdlib.h>

typedef struct __START_END {
    int start;
    int end;
} StartEnd;

void getArgs(int argc, char *argv[], int *numThreads, int *numElts);

int main (int argc, char *argv[]) {
    // Get arguments
    int numThreads, numElts = 0;
    getArgs(argc, argv, &numThreads, &numElts);

    printf("Num threads: %d, Num elements: %d\n", numThreads, numElts);

    // Allocate array
    int *ar = malloc(numElts * sizeof(int));

    // Clean up
    free(ar);
}


/**
 * Get numThreads and numElts from command line args.
 * 
 * @param int argc: Length of argv
 * @param char *argv[]: Command line arguments
 * @param int *numThreads: Gets assigned the number of threads
 * @param int *numElts: Gets assigned the number of elements
 */
void getArgs(int argc, char *argv[], int *numThreads, int *numElts) {
    if (argc != 3) {
        printf("Only two arguments are allowed. Exiting...\n");
        exit(1);
    } else {
        char *eptr;
        *numThreads  = strtol(argv[1], &eptr, 10);
        if (*numThreads == 0 && eptr == argv[1]) {
            printf("Error converting %s to int. Exiting...\n", argv[1]);
            exit(1);
        }
        *numElts = strtol(argv[2], &eptr, 10);
        if (*numElts == 0 && eptr == argv[2]) {
            printf("Error converting %s to int. Exiting...\n", argv[2]);
            exit(1);
        }
    }
}
