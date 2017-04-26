/*smash.c
main file. This file contains the main function of smash
*******************************************************************/
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h> 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include "commands.h"
#include "signals.h"
#define MAX_LINE_SIZE 80
#define MAXARGS 20

char* L_Fg_Cmd;
Plist jobs;  //This represents the list of jobs. Please change to a preferred type (e.g array of char*)
char lineSize[MAX_LINE_SIZE];
job fg_job;
//**************************************************************************************
// function name: main
// Description: main function of smash. get command from user and calls command functions
//**************************************************************************************
int main(int argc, char *argv[])
{
    char cmdString[MAX_LINE_SIZE]; 	   

	
	//signal declaretions
	//NOTE: the signal handlers and the function/s that sets the handler should be found in siganls.c
	 /* add your code here */
    
    struct sigaction int_signal;
    struct sigaction stp_signal;
    struct sigaction chld_signal;
    int_signal.sa_handler = handler_ctrlc;
    stp_signal.sa_handler = handler_ctrlz;
    chld_signal.sa_handler = handler_chld;
    chld_signal.sa_flags = SA_NOCLDSTOP;

	/************************************/
	//NOTE: the signal handlers and the function/s that sets the handler should be found in siganls.c
	//set your signal handlers here
	/* add your code here */

	/************************************/
    
    sigaction(SIGINT,  &int_signal,  NULL);
    sigaction(SIGTSTP, &stp_signal,  NULL);
    sigaction(SIGCHLD, &chld_signal, NULL);

	/************************************/
	// Init globals 
    //reset_job(&cur_job); 
    jobs = init_list();
    //reset_job(fg_job);
    //add_job(jobs, "test", 100, ACTIVE);
    //send_job_to_bg(jobs, 100);

	L_Fg_Cmd =(char*)malloc(sizeof(char)*(MAX_LINE_SIZE+1));
	if (L_Fg_Cmd == NULL) 
			exit (-1); 
	L_Fg_Cmd[0] = '\0';
	
    	while (1)
    	{
	 	printf("smash > ");
		fgets(lineSize, MAX_LINE_SIZE, stdin);
		strcpy(cmdString, lineSize);    	
		cmdString[strlen(lineSize)-1]='\0';
					// perform a complicated Command
		if(!ExeComp(jobs,lineSize)) continue;
					// background command	
	 	if(!BgCmd(lineSize, jobs)) continue;
					// built in commands
		ExeCmd(jobs, lineSize, cmdString);
		
		/* initialize for next line read*/
		lineSize[0]='\0';
		cmdString[0]='\0';
	}
    free (L_Fg_Cmd);
    destroy_list(jobs);
    return 0;
}

