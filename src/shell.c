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
#include "minishell.h"

char *lookupPath(char **);
int parseCommand(char *, struct command_t *);
void parsePath();
void printPrompt();
void readCommand(char* buffer);

char *dirs[MAX_PATHS]; // all directories within PATH
int pathLen = 0; // number of directories in dirs[]

int main() {
    
    parsePath();

    while(1) {
        printPrompt();

        struct command_t command;
        
        // read command line input then parse
	    char commandLine[Line_LEN];
        readCommand(commandLine);
        parseCommand(commandLine, &command);

        // get full pathname of file
        command.name = lookupPath(command.argv);
        if (command.name == NULL) {
            // report error
            perror("Command not found");
            continue;
        }

        // create a string that is the command and all args
        char commandStr[100];
        strcat(commandStr, command.name);
        strcat(commandStr, " ");
        for (int i=1; i<command.argc; i++){
            strcat(commandStr, command.argv[i]);
            strcat(commandStr, " ");
        }

        // create child and execute command
        if (fork() == 0) {
            system(commandStr);
            memset(commandStr, 0, strlen(commandStr));
        } else {
            wait(0);
        }
        // wait for child to execute
    }
    
    return 0;
}

void parsePath() {
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
        pathLen++;
    }

}

char *lookupPath(char **argv) {
    /*
     * This function searches the directories identified by the dir argument 
     * to see if argv[0] (the file name) appears there.
     * Allocate a new string, place the full path name in it, the return the string.
     */
    
    // check if argument name is already an absolute path name
    if (*argv[0] == '/') {
        if (access(argv[0], F_OK ) == 0) {
            return argv[0];
        } else {
            fprintf(stderr, "%s: command not found\n", argv[0]);
            return NULL;
        }
    }

    // Look in PATH directories.
    for (int i = 0; i < pathLen; i++) {
        
        // create the absolute path for the given command
        char *temp = (char *) malloc(100);
        strcpy(temp, dirs[i]);
        strcat(temp, "/");
        strcat(temp, argv[0]);
        
        // Use access() to see if the file is in a directory
        if (access(temp, F_OK ) == 0) {
            return temp;
        } else {
            free(temp);
        }
    }

    // File name not found in any path variable
    fprintf(stderr, "%s: command not found\n", argv[0]);
    return NULL;
}

void readCommand(char *buffer) {
    
    fgets(buffer, Line_LEN, stdin);//we use fgets over gets() because it is a safer function as it checks the entered command against the size regulation in our minishell =k
	return;
}


int parseCommand(char *cLine, struct command_t *cmd) {
		int argc;
		char **clPtr;
		
		//Initialization
		clPtr = &cLine; //This is the command line
		argc = 0;
		cmd->argv[argc] = (char *) malloc(MAX_ARG_LEN);
		//Fill argv[]
		while((cmd->argv[argc] = strsep(clPtr, WHITESPACE)) != NULL){
			cmd->argv[++argc] = (char *) malloc(MAX_ARG_LEN);
			}
			
		//Set command name and argc
		cmd->argc = argc-1;
		cmd->name = (char *) malloc(sizeof(cmd->argv[0]));
		strcpy(cmd->name, cmd->argv[0]);
		
		return 1;
}


void printPrompt() {
    char hostname[255];//this variable will hold the host's name, the maximum size of which is 255 characters =k
    gethostname(hostname, 255);//this command actually gets the host's name and stores it in the hostname variable =k
    
    char* username = getenv("USER");//this command retrieves the user's name and stores it in the username variable =k
    char* filepath = getenv("PWD"); //PWD-HOME //this command retrieves the user's current directory and stores it in the variable filepath =k
    
    printf("%s@%s:%s$ ", username, hostname, filepath); //this command prints out all the neccessary previously gathered information =k
}
