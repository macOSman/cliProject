/*
 * =====================================================================================
 *
 *       Filename:  shell.c
 *
 *    Description: A simple interactice shell implementation for COSC 4302 Operating
 *                 Systems. 
 *
 *        Version:  1.0
 *        Created:  11/17/2020 02:01:22 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Kalan Bonnette, Ryan Campbell, and Brent Hebert
 *   Organization:  Lamar University, COSC 4302-48F Operating Systems 
 *
 * =====================================================================================
 */
#include <stdlib.h>
#include "minishell.h"

char *lookupPath(char **, char **);
int parseCommand(char *, struct command_t *);
int parsePath(char **);
void printPrompt();
void readCommand();

int main() {

    while(1) {
        printPrompt();

        // read command line input then parse
        readCommand(commandLine);
        parseCommand(commandLine, &command);

        // get full pathname of file
        command.name = lookupPath(command.argv, pathv);
        if (command.name == NULL) {
            // report error
            continue;
        }

        // create child and execute command
        // wait for child to execute
    }
    
    return 0;
}

int parsePath(char *dirs[]) {
    /*
     * This function reads the PATH variable for this environment,
     *  then builds an array, dirs[], of the directories in PATH
     */
    char *pathEnvVar;
    char *thePath;

    for (i=0; i<MAX_ARGS; i++) {
        dirs[i] = NULL;
    }
    
    pathEnvVar = (char *) getenv("PATH");
    thePath = (char *) malloc(strlen(pathEnvVar) + 1);
    strcpy(thePath, pathEnvVar);

    // loop to parse PATH with ':' as delimiter between each path name

}

char *lookupPath(char **argv, char **dir) {
    /*
     * This function searches the directories identified by the dir argument 
     * to see if argv[0] (the file name) appears there.
     * Allocate a new string, place the full path name in it, the return the string.
     */
    char *result;
    char pName(MAX_PATH_LEN);

    // check if file name is already an absolute path name
    if (*argv[0] == '/') {
        // ...
        break;
    }

    // Look in PATH directories.
    // Use access() to see if the file is in a directory
    for (i = 0; i < MAX_PATHS; i++) {
        // ...
        break;
    }

    // File name not found in any path variable
    fprintf(stderr, "%s: command not found\n", argv[0]);
    return NULL;
}

void readCommand(char *buffer) {
    /*
     * Read entire command line into the buffer
     */
    gets(buffer);
}

void printPrompt() {
    // print string contains machine name, current directory, and other desired information
    promptString = "$";
    printf("$s ", promptString);
}
