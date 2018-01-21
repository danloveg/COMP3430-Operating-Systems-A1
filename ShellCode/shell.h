/*
 * shell.h
 *
 * Author: Daniel Lovegrove
 *
 * Version: January 20/2018
 *
 * Definitions for shell.c
 */

#ifndef SHELL_C

#define SHELL_C

#define MAX_INPUT_LEN 80
#define DELIMITER " "
#define SET_COMMAND "set"

void freeArray(void **ary, int len);
int countTokens(const char *str, const char *del);
void getCommandWithArgs(char *input, char *del, char *cmd, char ***args, int *arglen);
void executeUserCommand(char *cmd, char ***args, int arglen);

#endif
