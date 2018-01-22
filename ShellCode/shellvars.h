/*
 * shellvars.h
 *
 * Author: Daniel Lovegrove
 *
 * Version: January 22/2018
 *
 * Definitions for shellvars.c
 */

#ifndef SHELL_VARS_C

#define SHELL_VARS_C

#define INTERACTIVE 1
#define SUPPRESS_OUTPUT 2

#include "shell.h"

void initShellVarProg();
void quitShellVarProg();
void addShellVar(char *key, char *value);
void tryAddVariable(char *varStatement, int options);
bool substituteShellVariables(char ***args, int arglen);
char * getShellVar(char *key);

#endif
