/*
 * shellpipe.c
 *
 * Author: Daniel Lovegrove
 *
 * Version: January 22/2018
 *
 * Purpose: Provide different ways to execute a users's command.
 */

#include <assert.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include "shellpipe.h"


/**
 * Execute a command by piping its output to another executed commands input via
 * a pipe.
 * 
 * @param char *cmd1: Program 1's base command
 * @param char ***args1: Program 1's array of arguments
 * @param char *cmd2: Program 2's base command
 * @param char ***args2: Program 2's array of arguments
 * @returns int: The return status of the programs
 */
int executePipeToProgram(char *cmd1, char ***args1, char *cmd2, char ***args2) {
    int fd[2];
    int pid, returnStatus;

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
    }

    return returnStatus;
}
