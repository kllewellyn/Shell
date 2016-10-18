
/*
 * thsh.c
 *
 *  Created on: Sep 13, 2016
 *      Author: kayleellewellyn
 */
/* COMP 530: Tar Heel SHell */
/*
 * Honor Pledge: I pledge I have neither given nor received any
 * unauthorized help on this assignment
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include <fcntl.h>

// Assume no input line will be longer than 1024 bytes
#define MAX_INPUT 1024

int main (int argc, char ** argv, char **envp) {

  int finished = 0;
  char *prompt = "thsh> ";
  char cmd[MAX_INPUT];
  char *args[MAX_INPUT];
  int pid;
  char lastwd[1024];
  char printcwd[1024];
  int debug = 0;
  int exitStat = 0;
  char *newEnv = "?";
  char stringVal;
  char *val = 0;
  int inputHandle = 0;

  //Flag detecting if shell is in debug mode
  if(argv[1] != NULL && strstr(argv[1], "-d")){
	  debug = 1;
  }


  //Attempting to add scripting support by creating a flag
  else if(argv[1] != NULL && debug == 0){
      	inputHandle = open(*(argv+1), 4);
      	debug = 2;
      	if(inputHandle == -1){
      		printf("Script does not exist");
      		exitStat = 1;
      		debug = 0;
      	}

  }
  //Creates new environment variable $?
  stringVal = exitStat;
  val = &stringVal;
  setenv(newEnv, val, 1);

  while (!finished) {
    char *cursor;
    char last_char;
    int rv;
    int count;

    //Update the $? variable with most recently executed command
    stringVal = exitStat;
    val = &stringVal;
    setenv(newEnv, val, 1);


    //Memsetting the argument array
    memset(args, 0, sizeof(args));


    //  Print the prompt and the current working directory IF not in script mode
    if(debug == 0 || debug == 1){
    	prompt = malloc(MAX_INPUT * sizeof(char));
    	memset(prompt, 0, sizeof(prompt));
    	char *currwd = getcwd(printcwd, 1024);
    	prompt = strcat(prompt, "[");
    	prompt = strcat(prompt, printcwd);
    	prompt = strcat(prompt, "] ");
    	prompt = strcat(prompt, "thsh> ");
    	prompt = strcat(prompt, "\0");

    	rv = write(1, prompt, strlen(prompt));
    	if (!rv) {
    		finished = 1;
    		break;
    	}
    }
    	// read and parse the input
        for(rv = 1, count = 0, cursor = cmd, last_char = 1;
                    rv && (++count < (MAX_INPUT-1)) && (last_char != '\n'); cursor++) {
          rv = read(inputHandle, cursor, 1);
          last_char = *cursor;
        }
        *cursor = '\0';

        if (!rv) {
          finished = 1;
          exitStat = close(inputHandle);			//closes file if one was opened
          break;
        }


    //   Execute the command, handling built-in commands separately

    //Replace the \n character in cmd with \0
            int length = strlen(cmd);
            cmd[length-1] = '\0';

    //Parse out command and args by space or = delimiter
            int i = 0;
            char *temp;
            temp = strtok(cmd, "= ");
            while(temp != NULL){
            		args[i] = temp;
                    temp = strtok(NULL, " =");
                    i++;
            }
            args[i] = NULL;

            		//want it to reprint prompt if there's a blank cmd and do nothing
            		if(args[0] == 0){

            		}

            		//Exits if exit is called
            		else if(strstr(args[0], "exit")){
                            exit(EXIT_SUCCESS);
                    }

            		//sets a new env variable
                    else if(strstr(args[0], "set")){
                    	exitStat = setenv(args[1], args[2], 1);
                    	fprintf(stderr, "ENDED: %s (ret = %d)\n", args[0], exitStat);
                    }

            		//My test run for other commands with ls "my test playground" of sorts
                    else if(strstr(cmd, "ls")){

                    	if(debug == 1){
                    	       fprintf(stderr, "RUNNING: %s\n", cmd);
                    	}

                        if(pid == 0){
                                 //   printf("%s", cmd);
                                    //fflush(stdout);
                             if (execvp(args[0], args) == -1){
                                    fprintf(stderr, "execvp failed with cmd %s\n", cmd);
                             }

                             exit(EXIT_SUCCESS);
                        }
                        else{
                        exitStat = wait(pid);

                        if(debug == 1){
                            fprintf(stderr, "ENDED: %s (ret = %d)\n", cmd, EXIT_SUCCESS);
                        }
                        }
                    }

                    else if(strstr(cmd, "<") || strstr(cmd, ">") || strstr(cmd, "|")){
                    	if(debug == 1){
                    	   fprintf(stderr, "RUNNING: %s\n", cmd);
                    	}											//Attempting to add piping support
                    	int i = 0;
                    	pid = fork();
                    	if(pid == 0){
                    		while(args[i] != NULL){
                    			if(strstr(args[i+1], "<")){
                    				int fileHandle = open(args[i+2], 4);
                    				dup2(fileHandle, 0);
                    				close(fileHandle);

                    			}
                    			else if(strstr(args[i+2], ">")){			//This only words for commands in form of
                    				char *fileName = strtok(args[i+1], ">");	// ls -l >filename

                    				int fileHandle = open(fileName, O_WRONLY || O_CREAT);
                    				args[i+1] = NULL;
                    				dup2(fileHandle, 1);
                    				close(fileHandle);

                    			}
                    			execvp(args[i], args);
                    			i+=3;
                    			exit(EXIT_SUCCESS);

                    		}
                    	}
                    	else{
                    		exitStat = wait(pid);
                    		if (debug == 1) {
                    			fprintf(stderr, "ENDED: %s (ret = %d)\n", cmd, exitStat);
                    		}
                    	}

                    }

                    else if(strstr(args[0], "echo")){			//Adds support for echo to display variable
                    	if(debug == 1){
                    	     fprintf(stderr, "RUNNING: %s\n", args[0]);
                    	}
                    	if(strstr(args[1], "$")){				//value
                    		char *variable = strtok(args[1], "$");
                    			if(getenv(variable) != NULL){
                    				char *path = getenv(variable);
                    				printf("%s\n", path);
                    			}else{
                    				fprintf(stderr, "Variable %s not found \n", variable);
                    			}
                    	}
                    	else{
                    		int k = 1;							//Otherwise just prints back what was written
                    		while(args[k] != NULL){
                    			printf("%s ", args[k]);
                    			k++;
                    		}
                    		printf("\n");
                    		fflush(stdout);
                    	}
                    	if (debug == 1) {
                    		fprintf(stderr, "ENDED: %s (ret = %d)\n", args[0], exitStat);
                    	}
                    }

            		//Support cd
                    else if(strstr(args[0], "cd")){
                    	if(debug == 1){				//Prints debug messages when in debug mode
                    	       fprintf(stderr, "RUNNING: %s\n", args[0]);
                    	}

                    		if(strstr(args[1], "-")){			//Accounts for cd - to navigate between directories
                    			exitStat = chdir(lastwd);
                    			char *last = getcwd(lastwd, 1024);
                    		}
                    		else if(strstr(args[1], "-") || args[1] == 0 || strstr(args[1], "~") || strstr(args[1], ".")){		//Allows you to navigate to home dir
                    			char *home = "$HOME";
                    			char *homeDir = getenv(home);
                    			chdir(home);
                    		}
                    		else{
                    			char *last = getcwd(lastwd, 1024);		//saves last working directory
                    			exitStat = chdir(args[1]);					//changes directory
                    		}
                    	if(debug == 1){
                    		 fprintf(stderr, "ENDED: %s (ret = %d)\n", args[0], exitStat);
                    	}

                    }

            //If command doesn't have filepath then exec
		else if (!strstr(cmd, "/")) {
			if (debug == 1) {			//Prints debug messages when in debug mode
				fprintf(stderr, "RUNNING: %s\n", args[0]);
			}
			pid = fork();
			if (pid == 0) {
				if (execvp(args[0], args) == -1 && debug != 2) {
                            printf("command not found\n");
                            fflush(stdout);
				}
				exit(EXIT_SUCCESS);
            }
			else{
				exitStat = wait(pid);

			if (debug == 1) {
				fprintf(stderr, "ENDED: %s (ret = %d)\n", args[0], exitStat);

			}
			}
    }


// Just echo the command line for now
//write(1, cmd, strnlen(cmd, MAX_INPUT));

}

return 0;
}
