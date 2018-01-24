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
#include <unistd.h>

#define FILENAME "./numbers.txt"

typedef struct __START_END {
    int start;
    int end;
} StartEnd;

void getArgs(int argc, char *argv[], int *numThreads, int *numElts);
int findMin(unsigned int threadNumElts, unsigned int start, const int *ar);
void loadArrayFromFile(const char *filename, int *ar, int numElts);

// Define static array
int *ar;

int main (int argc, char *argv[]) {
    // Get arguments
    int numThreads, numElts = 0;
    getArgs(argc, argv, &numThreads, &numElts);

    printf("Num threads: %d, Num elements: %d\n", numThreads, numElts);

    // Allocate array and fill it
    ar = calloc(numElts, sizeof(int));
    loadArrayFromFile(FILENAME, ar, numElts);

    // Clean up
    free(ar);
}


/**
 * Load the array with integers from the file.
 * 
 * @param const char *filename: The name of the file. Must be a relative or full
 *     path
 * @param int *ar: The array to file
 * @param int numElts: The number of elements in the file
 */
void loadArrayFromFile(const char *filename, int *ar, int numElts) {
    char charBuf[64];
    char *characterBuffer = &charBuf[0];
    char *eptr;
    char c;
    int arIndex, lineIndex, element = 0;
    FILE *filePtr = NULL;

    // Check to see if we can open the file
    if (access(filename, F_OK) != -1) {

        filePtr = fopen(filename, "r");
 
        if (filePtr != NULL) {
            // File is opened. Start processing.
            for (arIndex = 0; arIndex < numElts; arIndex++) {

                lineIndex = 0;
                c = fgetc(filePtr);

                while(c != EOF && c != '\n') {
                    characterBuffer[lineIndex++] = c;
                    c = fgetc(filePtr);
                }
                characterBuffer[lineIndex] = '\0';

                // Get an integer from the string
                element = strtol(characterBuffer, &eptr, 10);

                if (element == 0 && eptr == characterBuffer) {
                    printf("Error at file line %d, check to make sure val is an int.\nExiting...\n", arIndex + 1);
                    exit(1);
                } else {
                    ar[arIndex] = element;
                }
            }
            fclose(filePtr);
        }
    } else {
        printf("Error: Could not open %s\n", filename);
    }
}


/**
 * Find the minimum number in an array of ints.
 * 
 * @param unsigned int threadNumElts: The number of elements to look through
 * @param unsigned int start: The starting index to look from
 * @param const int *ar: Int array
 */
int findMin(unsigned int threadNumElts, unsigned int start, const int *ar) {
    int i;
    int min = ar[start];

    for (i = start + 1; i < threadNumElts; i++) {
        if (ar[i] < min) {
            min = ar[i];
        }
    }

    return min;
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
