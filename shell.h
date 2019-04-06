#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/types.h>

// Non-Built-In Libraries:
#include "hashtable/uthash.h"

#define PROMPTCOLOR "\x1B[1;36m" 
#define MAX_CHARACTERS 1024
#define NUM_BUILTINCMDS 3

// Struct to handle History cmds (Hash Table):
struct command { 
    int id; // Hash key
    char name[1024];
    UT_hash_handle hh;
};

static char* builtInCmds[NUM_BUILTINCMDS]; // Built-In Commands
static int backgroundProcessFlag = 0; // Specifies if "&" is present
struct command *history = NULL; // Works as the hash structure


// PROGRAM FUNCTIONS:
void initShell();

int divideComposedCommand(char* command, char** parsedFullCommand);

char **parseSingleCommand(char* command);

int executeSingleCommand(char** parsedCommand);

int executeCommandWithPipe(char** parsedCommand, char** parsedPipeCommand);

void addCommandToHistory(int id, char *name);

struct command *findCommandInHistory(int id);

void printCommandByID(int key);

void printHistory();

int builtInHandler(char* command);

int executeBuiltInCmd(char* command, int builtInCommand);