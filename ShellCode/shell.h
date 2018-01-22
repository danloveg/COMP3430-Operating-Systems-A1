/*
 * shell.h
 *
 * Author: Daniel Lovegrove
 *
 * Version: January 21/2018
 *
 * Definitions for shell.c
 */

#ifndef SHELL_C

#define SHELL_C

#define MAX_INPUT_LEN 80
#define DELIMITER " "
#define SET_COMMAND "set"

void freeArray(void **ary, int len);
void loadShellVariablesFromFile();
void setShellVariableFromArgs(char *cmd, char ***args, int arglen);
void getCommandWithArgs(char *input, char *del, char *cmd, char ***args, int *arglen);
void executeUserCommand(char *cmd, char ***args, int arglen);

typedef enum __BOOL { false, true } bool;

#endif
