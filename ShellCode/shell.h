/*
 * shell.h
 *
 * Author: Daniel Lovegrove
 *
 * Version: January 22/2018
 *
 * Definitions for shell.c
 */

#ifndef SHELL_C

#define SHELL_C

#define MAX_INPUT_LEN 80
#define DELIMITER " "
#define SET_COMMAND "set"

typedef enum __BOOL { false, true } bool;
typedef enum __PIPE_OP { nopipe, fileoverwrite, fileappend, fullpipe } pipeoperation;

void freeArray(void **ary, int len);
void loadShellVariablesFromFile();
void setShellVariableFromArgs(char *cmd, char ***args, int arglen);
void getCommandWithArgs(char *input, char *del,
    char **cmd1, char ***args1, int *arglen1, char **cmd2, char ***args2, int *arglen2, pipeoperation *pipeop);
void executeUserCommand(char *cmd1, char ***args1, int arglen1, pipeoperation pipeop, char *cmd2, char ***args2, int arglen2);

#endif
