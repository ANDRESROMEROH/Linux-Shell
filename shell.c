#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <wait.h>

#define PROMPT "Shell> "
#define MAX_CHARACTERS 1024
#define NUM_BUILT_IN_CMDS 2

static char* builtInCmds[NUM_BUILT_IN_CMDS]; //Array of Built-In Commands


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
        parsedCommand[iterator] = token;             
        iterator ++;                          
        token = strtok(NULL, " \t\r\n\a");  
    }

    parsedCommand[iterator] = NULL;

    return parsedCommand;
}

int executeCommand(char** parsedCommand){
    pid_t pid;
    pid = fork();

    if (pid < 0) {
        printf("There was an error calling fork()\n");
        return -1;
    }
    else if (pid == 0) {
        //execlp(command,command,NULL);
        execvp(parsedCommand[0], parsedCommand);
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
    int executionFlag = 1;

    initShell(); // Executing init() function...

    while (executionFlag == 1) { 

        printf("%s",PROMPT); // Print Shell PROMPT
        fflush (stdout); // Clear (or flush) the output buffer
        fgets(command,MAX_CHARACTERS,stdin); // Get user input
        command[strcspn(command, "\n")] = '\0'; // Remove 'ENTER'

        if (strlen(command) > 1) {

            if (builtInHandler(command) == 0) {
                parsedCommand = parseCommand(command);
                executeCommand(parsedCommand);
            }
            
        }
    } 
    

    return 0;
}
