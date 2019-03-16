#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <regex.h>
#include <stdio.h>
#include <wait.h>

#define PROMPTCOLOR  "\x1B[1;32m" 
#define MAX_CHARACTERS 1024
#define NUM_BUILT_IN_CMDS 2

static char* builtInCmds[NUM_BUILT_IN_CMDS]; //Array of Built-In Commands
static int executionFlag = 1; //Flag for executing the main() process
static int backgroundProcessFlag = 0; //Specifies if "&" is present

#pragma region FUNCTIONS:
void initShell(){
    // Initiate Built-In commands:
    builtInCmds[0] = "exit"; 
    builtInCmds[1] = "history";

    printf("\033[H\033[J"); //Clear Screen
}

char** parseCommand(char* command){
    char **parsedCommand = malloc (sizeof(char *) * 1024);
    char *token = malloc (sizeof(char) * MAX_CHARACTERS);
    int iterator = 0;

    token = strtok(command," \t\r\n\a");

    while(token != NULL) //Break command into single parts, Ex: ls -l -> [ls] [-l]
    {
        // If & is included process running in background:
        if(strcmp(token, "&") == 0){
            backgroundProcessFlag = 1;
            iterator ++;                          
            token = strtok(NULL, " \t\r\n\a");  
        } else {
            parsedCommand[iterator] = token;             
            iterator ++;                          
            token = strtok(NULL, " \t\r\n\a");  
        }
    }

    parsedCommand[iterator] = NULL;

    return parsedCommand;
}

int executeCommand(char** parsedCommand, char backgroundProcessIndicator){
    pid_t pid;
    pid = fork();

    if (pid < 0) {
        printf("There was an error calling fork()\n");
        return -1;
    }
    else if (pid == 0) {
        if (execvp(parsedCommand[0], parsedCommand) == -1) {
            printf("%s: command not found\n", parsedCommand[0]);
        }
    }
    else {
        wait(NULL);
        return 0;
    }
}

int builtInHandler(char* command){
    int builtInCommand;

    for(size_t i = 0; i < NUM_BUILT_IN_CMDS; i++)
    {
        if (strcmp(command, builtInCmds[i]) == 0) { 
            builtInCommand = i + 1; 
            break; 
        } 
    }
    
    switch (builtInCommand)
    {
        case 1: //Exit
           exit(0); // Finish the main() process
    
        default:
            break;
    }

    return 0;
}


/* Main Function: */
int main(int argc, char const *argv[])
{
    char* command = malloc(sizeof(char) * MAX_CHARACTERS);
    char** parsedCommand = malloc (sizeof(char *) * MAX_CHARACTERS);

    initShell(); // Executing init() function...

    while (executionFlag == 1) { 

        printf("%sShell> \x1B[37m",PROMPTCOLOR); // Print Shell PROMPT
        fflush(stdout); // Clear (or flush) the output buffer
        fgets(command,MAX_CHARACTERS,stdin); // Get user input

        if (strlen(command) > 1) {

            command[strcspn(command, "\n")] = '\0'; // Remove 'ENTER'

            if (builtInHandler(command) == 0) {
                parsedCommand = parseCommand(command); // Break command into single parts, Ex: ls -l -> [ls] [-l]
                executeCommand(parsedCommand, backgroundProcessFlag); //Ex: cat ejemplo.c & or ls -l etc...
            }
            
        }
    } 
    

    return 0;
}
