# Basic Command Shell and Multithreading in C
### COMP 3430 Winter 2018 Assignment 1

## Overview
The main portion of the assignment is to extend the basic C shell code written in Lab 1 to add shell variables, shell variable initialization from file, and piping. Questions 4-7 are code-based. The rest of the assignment is answering other questions related to concurrency in a Unix environment, and coding with threads.

## Details
All of the code has only been tested on Linux (Scientific and Ubuntu) and cannot be guaranteed to work on Mac or Windows.

## Running the Code

### Questions 4 - 6

Each of these questions builds on the last. Question 4 includes the ability to set shell variables, question 5 adds shell variable initialization from a file, and question 6 allows a single pipe via |, >, or >>. The code in the directory shellCode contains the most recent additions.

1. From a terminal, change directory to ShellCode: `cd ShellCode`
2. Build the executable: `make`
3. Run the shell: `./shell`

The shell looks for variables in a file at ~/.shell_init, if this file is not present, the shell will start up with an error. You can set variables with the syntax `set $<name>=<value>`. Piping is allowed via >, >>, or |, but pipes do not work with the set command, since it is not a system command, but one I wrote, so it is executed differently.

### Question 7

This program uses a divide-and-conquer method to find the minimum element in an array of numbers in the file numbers.txt. This file can be replaced with any file named numbers.txt.

1. From a terminal, change directory to Q5: `cd Question7Code`
2. Build the executable: `make`
3. Run the program, with two arguments, numThreads and numElts. numThreads is the number of threads to use, numElts is the number of integers to read from the file numbers.txt. numThreads must divide numElts. `./minelement  <numThreads> <numElts>`
