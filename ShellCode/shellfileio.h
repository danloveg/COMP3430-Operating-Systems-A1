/*
 * shellfileio.h
 *
 * Author: Daniel Lovegrove
 *
 * Version: January 21/2018
 *
 * Definitions for shellfileio.c
 */

#ifndef SHELL_FILE_IO_C

#define SHELL_FILE_IO_C

#define SHELL_INIT_FILE_NAME ".shell_init"

int openShellInitFile();
void readFileString(char *str);
int closeShellInitFile();

#endif
