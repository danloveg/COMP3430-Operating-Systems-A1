/*
 * shell.c
 *
 * Author: Daniel Lovegrove
 *
 * Version: January 22/2018
 *
 * Purpose: Provide the functionality of a mini shell. Takes input until the
 * user enters CTRL-D at the prompt. Allows the use of shell variables, as well
 * as two command pipes and stdout redirection via > or >>.
 */


#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <errno.h>
#include <fcntl.h>
#include "shell.h"
#include "shellvars.h"
#include "shellstring.h"
#include "shellfileio.h"

// Internal prototypes
void getPipeop(char ***args, int arglen, char **pipeop, int *pipeind);
void createArgsFromInput(char *input, char *delim, char **cmd, char ***args, int *arglen);
void splitArgsByPipe(char ***args1, int *arglen1, char **cmd2, char ***args2, int *arglen2, int pipeIndex);


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
            getCommandWithArgs(userInput, DELIMITER, &command1, &args1, &argCount1, &command2, &args2, &argCount2, &pipeOperator);

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


//------------------------------------------------------------------------------
// GENERAL FUNCTIONS
//------------------------------------------------------------------------------

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
 * @param char **cmd2: If piped, a pointer to the second command string
 * @param char ***args2: If piped, a pointer to a dynamic array of the second
 *     arg list
 * @param int *arglen2: If piped, the length of args2
 * @param char **pipeop: If piped, the operator used. |, >, or >>
 */
void getCommandWithArgs(char *input, char *delim,
    char **cmd1, char ***args1, int *arglen1, char **cmd2, char ***args2, int *arglen2, char **pipeop) {

    int pipeIndex = -1;

    // Set second set of arguments to NULL
    *pipeop = NULL;
    *args2 = NULL;
    *arglen2 = -1;

    assert(input != NULL && "input string cannot be NULL");
    assert(delim != NULL && "delimiter cannot be NULL");

    // Get a complete list of the first command and all arguments
    createArgsFromInput(input, delim, cmd1, args1, arglen1);

    // Get a pipe operator (if there is one)
    getPipeop(args1, *arglen1, pipeop, &pipeIndex);

    // Split the args into two arrays if there's a valid pipe
    if (*pipeop != NULL && (*args1)[pipeIndex + 1] != NULL) {
        splitArgsByPipe(args1, arglen1, cmd2, args2, arglen2, pipeIndex);
    }
}


/**
 * Create a string command and string array for all arguments. The input string
 * is tokenized: the first token is put in cmd, AND the first element of args.
 * The rest of the tokens are placed in the args array in order, as well as a
 * NULL byte as the last item of args.
 *
 * Note: args is assigned a dynamically created array, so it must be freed.
 *
 * Example:
 * If
 *      input: "ls -l | wc -l", delim: " "
 * Then
 *      *cmd = "ls", **args = {"ls", "-l", "|", "wc", "-l", 0}, *arglen = 6
 *
 * @param char *input: The user's input as a string
 * @param char *delim: The delimiter between commands and args
 * @param char **cmd: Pointer to the command string
 * @param char ***args: Pointer to array of strings, gets assigned dynamic array
 * @param int *arglen: Gets assigned the length of the args array
 */
void createArgsFromInput(char *input, char *delim, char **cmd, char ***args, int *arglen) {
    char tokBuf[MAX_INPUT_LEN] = "";
    char *token = &tokBuf[0];
    int i;

    if (input != NULL && delim != NULL) {
        // We need to add one for the null item at the end of the array
        *arglen = countTokens(input, delim) + 1;
        assert(*arglen > 0);
        *args = malloc(*arglen * sizeof(char *));
        assert(*args != NULL);

        // Fill dynamic array, with a zero as the last item
        for (i = 0; i < *arglen; i++) {

            // Get token
            if (i == 0) {
                token = strtok(input, delim);
                assert(token != NULL);
                strcpy(*cmd, token);
                assert(*cmd != NULL);
            } else if (i < *arglen - 1) {
                token = strtok(NULL, delim);
                assert(token != NULL);
            }

            // Put token (or zero) into array
            if (i < *arglen - 1) {
                (*args)[i] = strdup(token);
                assert((*args)[i] != NULL);
            } else {
                (*args)[i] = malloc(sizeof(char));
                assert((*args)[i] != NULL);
                (*args)[i] = '\0';
            }
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
void executeUserCommand(char *cmd1, char ***args1, int arglen1, char *pipeop, char *cmd2, char ***args2, int arglen2) {
    int pid, returnStatus;
    int fileDescriptor = 0;
    int fd[2];

    assert(cmd1 != NULL);
    assert(args1 != NULL);
    assert(*args1 != NULL);

    if (strcmp(cmd1, SET_COMMAND) == 0) {
        // User used set command, try to set shell variable:
        setShellVariableFromArgs(cmd1, args1, arglen1);
    } else if (pipeop == NULL || strcmp(">", pipeop) == 0 || strcmp(">>", pipeop) == 0) {
        // Substitute any shell variables...
        bool substitutionPassed = substituteShellVariables(args1, arglen1);

        // Execute if all shell variables were valid
        if (substitutionPassed == true) {
            if (pipeop != NULL) {
                if (strcmp(">", pipeop) == 0) {
                    // Open file, create if doesn't exist.
                    fileDescriptor = open(cmd2, O_WRONLY|O_CREAT, 0666);
                } else if (strcmp(">>", pipeop) == 0) {
                    // Open file, create if doesn't exist.
                    fileDescriptor = open(cmd2, O_WRONLY|O_CREAT|O_APPEND, 0666);
                }
            }
            if (fileDescriptor != -1) {
                // Fork the current process
                pid = fork();
                assert(pid != -1);

                if (pid == 0) {
                    if (pipeop != NULL) {
                        dup2(fileDescriptor, STDOUT_FILENO);
                    }
                    returnStatus = execvp(cmd1, *args1);
                    exit(returnStatus);
                } else {
                    if (pipeop != NULL) {
                        close(fileDescriptor);
                    }
                    waitpid(pid, &returnStatus, 0);
                    assert(returnStatus != 11 && "execvp received NULL for one or more arguments");

                    if (returnStatus == 65280 && cmd1 != NULL) {
                        printf("Unrecognized command.\n");
                    }
                }
            } else {
                printf("Could not redirect stdout to the file.\n");
            }
        }
    } else if (pipeop != NULL && strcmp("|", pipeop) == 0 && *args2 != NULL && cmd2 != NULL) {
        // Substitute shell variables
        bool substitutionPassedArgs1 = substituteShellVariables(args1, arglen1);
        bool substitutionPassedArgs2 = substituteShellVariables(args2, arglen2);

        if (substitutionPassedArgs1 == true && substitutionPassedArgs2 == true) {
            // Create pipe and fork
            pipe(fd);
            pid = fork();
            assert(pid != -1);

            if (pid == 0) {
                // Fork again
                pid = fork();
                assert(pid != -1);

                if (pid == 0) {
                    // This process is to write data, so close read part of pipe
                    close(fd[READ]);
                    // Close stdout, duplicate with write part of pipe
                    dup2(fd[WRITE], STDOUT_FILENO);
                    close(fd[WRITE]);
                    // Execute the child program
                    returnStatus = execvp(cmd1, *args1);
                    exit(returnStatus);
                } else {
                    // This process is to read data, so close write part of pipe
                    close(fd[WRITE]);
                    // Close stdin, duplicate with read part of pipe
                    dup2(fd[READ], STDIN_FILENO);
                    close(fd[READ]);
                    // Execute the child program
                    returnStatus = execvp(cmd2, *args2);
                    exit(returnStatus);
                }
            } else {
                // Close all of the pipe, we are going to wait for processes to finish
                close(fd[READ]);
                close(fd[WRITE]);
                waitpid(pid, &returnStatus, 0);

                if (returnStatus == 65280 && cmd1 != NULL) {
                    printf("Unrecognized command.\n");
                }
            }
        }
    } else if (pipeop == NULL || *args2 == NULL || cmd2 == NULL) {
        printf("Cannot pipe into nothing.\n");
    }
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


//------------------------------------------------------------------------------
// SHELL VARIABLE SPECIFIC FUNCTIONS
//------------------------------------------------------------------------------

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


//------------------------------------------------------------------------------
// PIPE SPECIFIC FUNCTIONS
//------------------------------------------------------------------------------

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
 * Split the argument list into two lists, one of args before the pipe, one of
 * the arguments after the pipe. It splits the arguments at the index of
 * pipeIndex.
 *
 * Example:
 * If
 *      **args1 = {"ls", "|", "wc", 0}
 * Then
 *      **args1 = {"ls", 0, 0, 0}, *cmd2 = "ls", **args2 = {"wc", "-l", 0}, *arglen2 = 3
 *
 * @param char ***args1: The entire argument list
 * @param int *arglen1: The length of args1
 * @param char **cmd2: Gets assigned to pointer to second command
 * @param char ***args2: Gets assigned dynamic array of second argument list
 * @param int *arglen2: Gets assigned length of args2
 * @param int pipeIndex: The index of args1 where the pipe is
 */
void splitArgsByPipe(char ***args1, int *arglen1, char **cmd2, char ***args2, int *arglen2, int pipeIndex) {
    int i, j;

    *arglen2 = *arglen1 - pipeIndex - 1;
    assert(*arglen2 > -1);

    // We don't want the operator as a part of the arg list. So, get rid of it
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
