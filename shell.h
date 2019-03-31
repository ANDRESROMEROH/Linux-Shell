#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/types.h>
#include "hashtable/uthash.h"

#define PROMPTCOLOR  "\x1B[1;32m" 
#define MAX_CHARACTERS 1024
#define NUM_BUILTINCMDS 2

// Struct for handling commands in History (Hash Table):
struct command {
    int id; // key
    char name[10];
    UT_hash_handle hh;
};

static char* builtInCmds[NUM_BUILTINCMDS]; //Array of Built-In Commands
static int backgroundProcessFlag = 0; //Specifies if "&" is present
struct command *history = NULL; // Works as the hash structure

//Functions:
void initShell();
char **parseSingleCommand(char* command);
int divideComposedCommand(char* command, char** parsedFullCommand);
int executeSingleCommand(char** parsedCommand);
int executeCommandWithPipe(char** parsedCommand, char** parsedPipeCommand);
void addCommandToHistory(int id, char *name);
struct command *findCommandInHistory(int id);
void printHistory();
int builtInHandler(char* command);