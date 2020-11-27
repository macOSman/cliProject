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
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "minishell.h"

char *lookupPath(char **, char **);
int parseCommand(char *, struct command_t *);
void parsePath(char **);
void printPrompt();
void readCommand(char* buffer);

int main() {

    // Array of all directories in user's $PATH
    char *dirs[MAX_PATHS]; 
    parsePath(dirs);
     
    // main shell loop
    while(1) {
        // print prompt after every command
        printPrompt();

        struct command_t command;
        
        // read command line input then parse
	    char commandLine[Line_LEN];
        readCommand(commandLine);
        parseCommand(commandLine, &command);

        // get full pathname of file
        command.name = lookupPath(command.argv, dirs);
        if (command.name == NULL) {
            // report error
            perror("Command not found");
            continue;
        } else if (strcmp(command.name, "cd") == 0) {
            // edge case to handle traditional 'cd' command
            chdir(command.argv[1]);
            continue;
        } 

        // create child and execute command
        if (fork() == 0) {
            execv(command.name, command.argv);
        } else {
            wait(0);
        }
    }
    
    return 0;
}

/* 
 * Reads the user's PATH variable and parses it into an array were each element is separated by a ':' 
 * Args: An array to fill with all pathnames in $PATH
 * Author: Ryan Campbell
 */
void parsePath(char **dirs) {
    /*
     * This function reads the PATH variable for this environment,
     *  then builds an array, dirs[], of the directories in PATH
     */
    char *pathEnvVar;
    char *thePath;

    pathEnvVar = (char *) getenv("PATH");
    strcat(pathEnvVar, ":/bin");
    thePath = (char *) malloc(strlen(pathEnvVar) + 1);
    strcpy(thePath, pathEnvVar);
    
    // loop to parse PATH with ':' as delimiter between each path name
    dirs[0] = strtok(thePath, ":");

    int i = 0;
    while (dirs[i] != NULL) {
        dirs[++i] = strtok(NULL, ":");
    }

}

/* 
 * Searches available directories for binary files (commands) issued by the user.  
 * Returns a string, if it exists, that is the aboslute path name for the given command. 
 * Args: takes the user supplied arguments and a list of directories to search
 * Author: Ryan Campbell
 */
char *lookupPath(char **argv, char **dirs) {
    
    // check if argument name is already an absolute path name
    if (*argv[0] == '/') {
        if (access(argv[0], F_OK ) == 0) {
            return argv[0];
        } else {
            fprintf(stderr, "%s: command not found\n", argv[0]);
            return NULL;
        }
    } else if (strcmp(argv[0], "cd") == 0){
        // edge case to handle traditional cd command
        return "cd";
    }

    // Look in each PATH directory.
    int i = 0;
    while (dirs[i] != NULL) {

        // create the potential absolute path for the given command
        char *temp = (char *) malloc(100);
        strcpy(temp, dirs[i]);
        strcat(temp, "/");
        strcat(temp, argv[0]);
        
        // Use access() to see if its the correct directory
        if (access(temp, F_OK ) == 0) {
            return temp;
        } else {
            free(temp);
        }
        
        i++;
    }
        
    // File name not found in any path variable
    fprintf(stderr, "%s: command not found\n", argv[0]);
    return NULL;
}


/* 
 * This reads the command from the user into a buffer
 * Returns nothing
 * Must be passed a char array as a buffer
 * Kalan Bonnette
 */
void readCommand(char *buffer) {
     
    // We use fgets over gets() because it is a safer function as it checks the entered command 
    // against the size regulation in our minishell 
    fgets(buffer, Line_LEN, stdin);
	return;
}

/* 
 * <Describe Function> 
 * <RETURNS>
 * <ARGUMENTS>
 * <AUTHOR>
 */
int parseCommand(char *cLine, struct command_t *cmd) {
    int argc;
    char **clPtr;
    
    // Initialization
    clPtr = &cLine; // This is the command line
    argc = 0;
    cmd->argv[argc] = (char *) malloc(MAX_ARG_LEN);
    
    // Fill argv[]
    while((cmd->argv[argc] = strsep(clPtr, WHITESPACE)) != NULL){
        cmd->argv[++argc] = (char *) malloc(MAX_ARG_LEN);
    }
    
    // Set command name and argc
    cmd->argc = argc-1;
    cmd->name = (char *) malloc(sizeof(cmd->argv[0]));
    strcpy(cmd->name, cmd->argv[0]);
    
    return 1;
}

/* 
 * This reads the evironment's user data and current directory and outputs it to the user's interface. 
 * This function prints text, but does not return anything
 * This function needs to be passed no arguments
 * Kalan Bonnette
 */
void printPrompt() {
    
    // this variable will hold the host's name, the maximum size of which is 255 characters 
    // this command actually gets the host's name and stores it in the hostname variable 
    char hostname[255];
    gethostname(hostname, 255);
    
    // this command retrieves the user's name and stores it in the username variable 
    char* username = getenv("USER");
    
    // The following retrieves the user's current directory 
    // and stores it in the variable filepath 
    char s[100];
    char* filepath = getcwd(s, 100);

    // this command prints out all the neccessary previously gathered information 
    printf("%s@%s:%s$ ", username, hostname, filepath);
}
