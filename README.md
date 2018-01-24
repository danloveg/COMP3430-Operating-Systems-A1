# COMP 3430 Winter 2018 Assignment 1 - Concurrency in Command Shells

The main portion of the assignment is to extend the basic C shell code written in Lab 1 to add shell variables, shell variable initialization from file, and piping. The rest of the assignment is answering other questions related to concurrency in a Unix environment, and coding with threads. All code was **only** tested on Linux (Ubuntu & Scientific Linux), code was not tested on Mac or Windows.

To run the shell program (answers Q4-Q6):
1. Change directory to ShellCode: `cd ShellCode`
2. Build the executable: `make`
3. Run the shell: `./shell`

_Notes about shell_: The shell looks for variables in a file at ~/.shell_init, if this file is not present, the shell will start up with an error. You can set variables with the syntax `set $<name>=<value>`. Piping is allowed via >, >>, or |, but pipes do not work with the set command, since it is not a system command, but one I wrote, so it is executed differently.

To run the minimum element program (answers Q7):
1. Change directory to Question7Code: `cd Question7Code`
2. Copy a file called numbers.txt to this location. numbers.txt is a list of integers on separate lines.
2. Build the executable: `make`
3. Run the program, with two arguments, numThreads and numElts. numThreads is the number of threads to use, numElts is the number of integers to read from the file numbers.txt. numThreads must divide numElts. `./minelement  <numThreads> <numElts>`
