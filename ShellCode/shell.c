/*
 * shell.c
 *
 * Author: Daniel Lovegrove
 *
 * Version: January 22/2018
 *
 * Purpose: Provide the functionality of a mini shell. Takes input until the
 * user enters CTRL-D at the prompt. Allows the use of shell variables, as well
 * as two command pipes and stdout redirection.
 */


#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include "shell.h"
#include "shellvars.h"
#include "shellstring.h"
#include "shellfileio.h"


void getPipeop(char ***args, int arglen, char **pipeop, int *pipeind);


int main(int argc, char *argv[]) {
    char inBuf[MAX_INPUT_LEN] = "";
    char commandBuf1[MAX_INPUT_LEN] = "";
    char commandBuf2[MAX_INPUT_LEN] = "";
    char pipeopBuf[3] = "";
    char *userInput = &inBuf[0];
    char *command1 = &commandBuf1[0];
    char *command2 = &commandBuf2[0];
    char *pipeOperator = &pipeopBuf[0];
    char **args1 = NULL;
    char **args2 = NULL;
    int argCount1;
    int argCount2;

    // Enable shell variables
    initShellVarProg();
    loadShellVariablesFromFile();

    printf("$ ");

    // Loop until we get CTRL-D
    while (fgets(userInput, MAX_INPUT_LEN, stdin) != NULL) {

        removeTrailingNewline(userInput);

        if (userInput[0] != '\0') {
            // Get command, arguments, and arguments length
            getCommandWithArgs(userInput, DELIMITER, &command1, &args1, &argCount1, &pipeOperator, &command2, &args2, &argCount2);

            // Execute the command
            executeUserCommand(command1, &args1, argCount1, pipeOperator, command2, &args2, argCount2);

            // Clean up for next iteration
            freeArray((void**) args1, argCount1);
            freeArray((void**) args2, argCount2);
        }

        printf("$ ");
    }

    printf("\n");

    quitShellVarProg();
    exit(0);
}


/**
 * Loads the shell variables from the file .shell_init at the user's home
 * directory.
 */
void loadShellVariablesFromFile() {
    if (openShellInitFile() == 0) {
        char *line = NULL;

        do {
            line = readFileString();
            tryAddVariable(line, SUPPRESS_OUTPUT);
            free(line);
        } while (line != NULL);

        closeShellInitFile();
    }
}


/**
 * Extract the user's command and arguments from the input string, as well as
 * the length of the argument array. If there was a pipe operator used, such as
 * | or >, then two sets of commands and args are returned. If no pipe operator,
 * then cmd2, args2, and arglen2 are NULL.
 *
 * NOTE: ***args1 and ***args2 are assigned pointers to dynamically allocated
 *     string arrays and so must be deallocated when finished with.
 *
 * @param char *input: The user's input string
 * @param char *delim: The delimiter demarcating arguments (normally a space)
 * @param char **cmd1: A pointer to a string that gets assigned to the command
 *     extracted from the input string
 * @param char ***args1: A pointer to an array of strings that gets assigned to
 *      a dynamically allocated string array containing the command line args
 * @param int *arglen1: The length of args1
 * @param char **pipeop: If piped, the operator used. |, >, or >>
 * @param char **cmd2: If piped, a pointer to the second command string
 * @param char ***args2: If piped, a pointer to an array of the second arg list
 * @param int *arglen2: If piped, the length of args2
 */
void getCommandWithArgs(char *input, char *delim,
    char **cmd1, char ***args1, int *arglen1, char **pipeop, char **cmd2, char ***args2, int *arglen2) {

    char tokBuf[MAX_INPUT_LEN] = "";
    char *token = &tokBuf[0];
    int i, j;
    int pipeIndex = -1;

    // Set second set of arguments to NULL
    *pipeop = NULL;
    *args2 = NULL;
    *arglen2 = -1;

    assert(input != NULL && "input string cannot be NULL");
    assert(delim != NULL && "delimiter cannot be NULL");

    if (input != NULL && delim != NULL) {
        // We need to add one for the null item at the end of the array
        *arglen1 = countTokens(input, delim) + 1;
        assert(*arglen1 > 0);
        *args1 = malloc(*arglen1 * sizeof(char *));
        assert(*args1 != NULL);

        // Fill dynamic array, with a zero as the last item
        for (i = 0; i < *arglen1; i++) {

            // Get token
            if (i == 0) {
                token = strtok(input, delim);
                assert(token != NULL);
                strcpy(*cmd1, token);
                assert(*cmd1 != NULL);
            } else if (i < *arglen1 - 1) {
                token = strtok(NULL, delim);
                assert(token != NULL);
            }

            // Put token (or zero) into array
            if (i < *arglen1 - 1) {
                (*args1)[i] = malloc(strlen(token) + 1);
                assert((*args1)[i] != NULL);
                strcpy((*args1)[i], token);
            } else {
                (*args1)[i] = malloc(sizeof(char));
                assert((*args1)[i] != NULL);
                (*args1)[i] = (char) 0;
            }
        }
    }

    getPipeop(args1, *arglen1, pipeop, &pipeIndex);

    // If we have a pipe, we need to do some pointer re-pointing
    if (*pipeop != NULL && (*args1)[pipeIndex + 1] != NULL) {
        *arglen2 = *arglen1 - pipeIndex - 1;
        assert(*arglen2 > -1);

        // We don't want the operator as a part of the list
        free((*args1)[pipeIndex]);
        (*args1)[pipeIndex] = NULL;
        *args2 = malloc(*arglen2 * sizeof(char*));
        assert(*args2 != NULL);
        strcpy(*cmd2, (*args1)[pipeIndex + 1]);

        // Redirect pointers
        for (j = pipeIndex + 1, i = 0; j < *arglen1; j++, i++) {
            (*args2)[i] = (*args1)[j];
            (*args1)[j] = NULL;
        }
    }
}


/**
 * Execute the user's command with arguments. Supports a single pipe. If command
 * is not piped, pass NULL for cmd2, and args2.
 *
 * @param char *cmd1: The string containing the user's first command
 * @param char ***args1: A pointer to an array of strings holding the user's
 *     first command along with arguments, and a zero at the end.
 * @param int arglen1: The length of args1
 * @param char *pipeop: If piped, the operator used. |, >, or >>
 * @param char *cmd2: If piped, the string containing the user's second command
 * @param char ***arg2: If piped, the second set of arguments
 * @param int arglen2: If piped, the length of args2
 *
 * Example cmd and args for execvp (because it can be tricky)
 *     cmd: "/bin/ls"
 *     args: {"/bin/ls", "-l", "-a", 0}
 */
void executeUserCommand(char *cmd1, char ***args1, int arglen1, char *pipeop, char *cmd2, char ***arg2, int arglen2) {
    int pid, returnStatus;

    assert(cmd1 != NULL);
    assert(args1 != NULL);
    assert(*args1 != NULL);

    if (strcmp(cmd1, SET_COMMAND) == 0) {
        // User used set command, try to set shell variable:
        setShellVariableFromArgs(cmd1, args1, arglen1);
    } else {
        // Substitute any shell variables...
        bool substitutionPassed = substituteShellVariables(args1, arglen1);

        // Execute if all shell variables were valid
        if (substitutionPassed == true) {
            // Fork the current process
            if ((pid = fork()) == 0) {
                returnStatus = execvp(cmd1, *args1);
                exit(returnStatus);
            } else {
                waitpid(pid, &returnStatus, 0);
                assert(returnStatus != 11 && "execvp received NULL for one or more arguments");

                if (returnStatus == 65280 && cmd1 != NULL) {
                    printf("Unrecognized command.\n");
                }
            }
        }
    }
}


/**
 * Set a new shell variable from the user's command and args.
 *
 * @param char *cmd: The user's command
 * @param char ***args: The user's arguments in execv format
 * @param int arglen: The length of the args array
 */
void setShellVariableFromArgs(char *cmd, char ***args, int arglen) {
    assert(cmd != NULL);
    assert(args != NULL);
    assert(*args != NULL);

    if (arglen != 3) {
        printf("set requires one and only one argument.\n");
    } else {
        tryAddVariable((*args)[1], INTERACTIVE);
    }
}


/**
 * Get a pipe operation from a string array and the index of it in the array.
 *
 * @param char ***args: The string array
 * @param int arglen: The length of the string array
 * @param char **pipeop: Pointer to pipe operation. Given NULL if no pipe
 * @param int *pipeind: Pointer to pipe index. Given -1 if no pipe
 */
void getPipeop(char ***args, int arglen, char **pipeop, int *pipeind) {
    *pipeind = -1;
    *pipeop = NULL;

    if ((*pipeind = getStringIndex(args, arglen, ">")) != -1) *pipeop = ">";
    else if((*pipeind = getStringIndex(args, arglen, ">>")) != -1) *pipeop = ">>";
    else if((*pipeind = getStringIndex(args, arglen, "|")) != -1) *pipeop = "|";
}


/**
 * Free a dynamically created array
 *
 * @param void *ary: The array to free
 * @param int len: The length of the array
 */
void freeArray(void **ary, int len) {
    int i;

    if (ary != NULL) {
        for (i = 0; i < len; i++) {
            if (ary[i] != NULL) {
                free(ary[i]);
                ary[i] = NULL;
            }
        }

        free(ary);
        ary = NULL;
    }
}
