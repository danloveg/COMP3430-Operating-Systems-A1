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
    int numElts;
    int *ar;
} StartEndArray;

void getArgs(int argc, char *argv[], int *numThreads, int *numElts);
int findMin(unsigned int threadNumElts, unsigned int start, const int *ar);
void loadArrayFromFile(const char *filename, int *ar, int numElts);
void *workerThread(void *arg);

// Static int array
int *ar;

int main (int argc, char *argv[]) {
    int i, numThreads, numElts, eltsPerThread, minimum = 0;
    int *returnValue;
    StartEndArray threadArg;

    // Get arguments
    getArgs(argc, argv, &numThreads, &numElts);
    printf("Using %d thread(s) to find minimum of %d element(s)...\n", numThreads, numElts);

    // Allocate arrays
    ar = calloc(numElts, sizeof(int));
    loadArrayFromFile(FILENAME, ar, numElts);

    int reducedIntBuf[numThreads];
    int *reducedArray = &reducedIntBuf[0];
    pthread_t threads[numThreads];
    eltsPerThread = numElts / numThreads;

    // Create threads
    for (i = 0; i < numThreads; i++) {
        threadArg.ar = ar;
        threadArg.start = i * eltsPerThread;
        threadArg.numElts = eltsPerThread;
        pthread_create(&threads[i], NULL, workerThread, &threadArg);
    }

    minimum = findMin(12, 12, ar);

    // Join threads and add their min to a reduced array
    for (i = 0; i < numThreads; i++) {
        pthread_join(threads[i], (void **) &returnValue);
        reducedArray[i] = *returnValue;
        free(returnValue);
    }

    // Get the minimum from the reduced array
    minimum = findMin(numThreads, 0, reducedArray);
    printf("MINIMUM VALUE = %d\n", minimum);

    // Clean up
    free(ar);
}


void *workerThread(void *arg) {
    StartEndArray *my_arg = (StartEndArray *) arg;

    // Find minimum value in array
    int *returnValue = malloc(sizeof(int));
    *returnValue = findMin(my_arg -> numElts, my_arg -> start, my_arg -> ar);

    return (void *) returnValue;
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
                    printf("Error at file line %d, check to make sure val is an int. Exiting...\n", arIndex + 1);
                    fclose(filePtr);
                    exit(1);
                } else {
                    ar[arIndex] = element;
                }
            }
            fclose(filePtr);
        }
    } else {
        printf("Error: Could not open file %s. Exiting...", filename);
        exit(1);
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

    for (i = start + 1; i < threadNumElts + start; i++) {
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
        if (*numElts % *numThreads != 0) {
            printf("Number of threads must evenly divide number of elements. Exiting...\n");
            exit(1);
        }
    }
}
