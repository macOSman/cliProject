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

char *lookupPath(char **, char **);
int parseCommand(char *, struct command_t *);
int parsePath(char **);
void printPrompt();
void readCommand();

char *dirs[MAX_PATHS];

int main() {
    // parsePath()

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

void parsePath() {
    /*
     * This function reads the PATH variable for this environment,
     *  then builds an array, dirs[], of the directories in PATH
     */
    char *pathEnvVar;
    char *thePath;

    pathEnvVar = (char *) getenv("PATH");
    thePath = (char *) malloc(strlen(pathEnvVar) + 1);
    strcpy(thePath, pathEnvVar);

    // loop to parse PATH with ':' as delimiter between each path name
    char *tempStr = strtok(pathEnvVar, ":");

    int i = 0;
    while (tempStr != NULL) {
        dirs[i++] = tempStr;
        tempStr = strtok (NULL, ":");
    }

}

char *lookupPath(char **argv, char **dir) {
    /*
     * This function searches the directories identified by the dir argument 
     * to see if argv[0] (the file name) appears there.
     * Allocate a new string, place the full path name in it, the return the string.
     */
    char result[100];
    char pName(MAX_PATH_LEN); // TODO: Not sure what this does

    // check if file name is already an absolute path name
    if (*argv[0] == '/') {
        if (access(argv[0], F_OK && X_OK) == 0) {
            return argv[0];
        } else {
            fprintf(stderr, "%s: command not found\n", argv[0]);
            return NULL;
        }
    }

    // Look in PATH directories.
    // Use access() to see if the file is in a directory
    for (int i = 0; i < MAX_PATHS; i++) {
        // create the absolute path for the given command
        char temp[100];
        strcat(temp, dirs[i]);
        strcat(temp, "/");
        strcat(temp, argv[0]);
        // test for existance and executablility
        if (access(temp, F_OK && X_OK)) {
            return temp;
        }
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

//Sets up command_t to save command arguments
struct command_t{
	char *name;
	int argc;
	char *argv[MAX_ARGS];
};

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
    
    printf("%s@%s:%s$", username, hostname, filepath); //this command prints out all the neccessary previously gathered information =k
}
