/*
 * shellvars.h
 *
 * Author: Daniel Lovegrove
 *
 * Version: January 20/2018
 *
 * Definitions for shellvars.c
 */

#ifndef SHELL_VARS_C

#define SHELL_VARS_C

void addShellVar(char *key, char *value);
char * getShellVar(char *key);

#endif
