/*
 * shellexecute.h
 *
 * Author: Daniel Lovegrove
 *
 * Version: January 21/2018
 *
 * Definitions for shellpipe.c
 */

#ifndef SHELL_PIPE_C

#define SHELL_PIPE_C

#define READ 0
#define WRITE 1
#define OVERWRITE 0
#define APPEND 1

int executeSingleCommand(char *cmd1, char ***args1);
int executePipeToFile(char *cmd, char ***args, char *filename, int operationType);
int executePipeToProgram(char *cmd1, char ***args1, char *cmd2, char ***args2);

#endif
