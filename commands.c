//		commands.c
//********************************************
#include "commands.h"

#define max_history 50
#define max_args 80
//********************************************
// function name: ExeCmd
// Description: interperts and executes built-in commands
// Parameters: pointer to jobs, command string
// Returns: 0 - success,1 - failure
//**************************************************************************************

//Globals : //todo :understand where i need to init this globals ?!
int history_idx; // = 0 ;
char history[max_history][max_args] ;//= {NULL};
char cd_prev[MAX_LINE_SIZE] ; // = NULL;
extern Plist jobs ;
extern job fg_job;

int ExeCmd(Plist jobs, char* lineSize, char* cmdString) //one cmd per time
{	
	int job_pid = 0;
	int job_num = 0;
	char* cmd; 
	char* args[MAX_ARG];
	char pwd[MAX_LINE_SIZE];
	char* delimiters = " \t\n";  
	int i = 0, num_arg = 0;
	bool illegal_cmd = FALSE; // illegal command
    	cmd = strtok(lineSize, delimiters);
	if (cmd == NULL)
		return 0; 
   	args[0] = cmd;
	for (i=1; i<MAX_ARG; i++)
	{
		args[i] = strtok(NULL, delimiters); 
		if (args[i] != NULL) 
			num_arg++; 
 
	}
	

//list method used: 
//num_of_jobs - the number of all the jobs in the list. --int
//job_num - needed index of the jobs in the list [1 , num_of_jobs]  -- starting by 1 !! 
//cmd_name - for example "b.out"    -- (they call it command_number)
//job_pid - string of the job pid;
//suspended - indicate if the process is suspended.

/*print_jobs(&jobs) - print all the jobs by the requsted format. -- return value void ? */

/*get_last_suspended(&jobs) - return the node of relevent node ; -- return job element */

/*find_job_by_idx(&jobs,job_num) - finding job by given job_num (implemented by iterators "for")
 -- return value is the ptr to node of the job -- return job element 
 -- example for use: 
jobs *target_job_for_kill = find_job_by_idx(&jobs,job_num) ;
int  pid_to_kill = target_job_for_kill -> job_pid ; */

/*find_job_by_pid(&jobs,job_pid) - just like the function above but finding the node by pid */

/*remove_job(job_pid) - remove job from the list -  todo:return 0 sucess  1 failed*/ 


 
 /*************************************************/
	//if not history cmd do this -- history shouldn't added and printed in the history list 
	if (strcmp(cmd, "history")) 
	{
		for (i=max_history-2 ; i>=0; i--) strcpy(&history[i+1][0], &history[i][0]);
        history_idx = (history_idx == 50) ? 50 : history_idx +1;
		strcpy(&history[0][0],cmdString);
	}
	/*************************************************/
	if (!strcmp(cmd, "cd") ) 
	{	
		if (num_arg != 1)  
		{	
			illegal_cmd = TRUE ;
		}
		else
		{			
			if (!strcmp(args[1], "-"))   	      	   //change to prev directiry
			{	
				//printf("\n%s\n1\n",cd_prev); //debug
				if(cd_prev != NULL)
				{
					char* temp_dir = getcwd(pwd, MAX_LINE_SIZE);
					chdir(cd_prev);
				  	strcpy(cd_prev,temp_dir) ;
				  	//cd_prev = temp_dir;
				}
			}
			else  	
			{	
				//printf("\n%s\n3\n",cd_prev); //debug
				char* temp_dir = getcwd(pwd, MAX_LINE_SIZE);
				if((chdir(args[1]))) 			// chdir return 0 for success //need-to see error code ?
				perror("smash error: > “path” - path not found");
				else
				strcpy(cd_prev,temp_dir);
				//printf("\n%s\n4\n",cd_prev); //debug
			}	
		}	
			
	} 
	
	/*************************************************/
	else if (!strcmp(cmd, "pwd")) 
	{
		if(num_arg !=0)
		{
			illegal_cmd = TRUE ;  //illegal input
		}
		else 
		{	
			getcwd(pwd, MAX_LINE_SIZE);
			printf("%s\n", pwd);
		}
		
	}
	
	else if (!strcmp(cmd, "jobs"))  //this function print 2D array ? no changed to list type
	{
 		if(num_arg !=0)
		{
			illegal_cmd = TRUE ;  //illegal input
		}
		else 
		{	
			//printf("111");
			print_jobs(jobs);
		}
	}
	/*************************************************/
	else if (!strcmp(cmd, "showpid")) 
	{
		if(num_arg !=0)
		{
			illegal_cmd = TRUE ; //illegal input
		}
		else 
		{
			job_pid = getpid();  
			printf("Smash pid is %d\n", job_pid);
		}
	}
	/*************************************************/
	else if (!strcmp(cmd, "history")) 
	{
		if(num_arg !=0)
		{
			illegal_cmd = TRUE ; //illegal input
		}
		else 
		{	//running 50 time print only if not NULL // FIFO Printing 
            for(i=0 ; i < history_idx ; i++) printf("%s\n",&history[i][0]);
		}
	} 
    
                 
	/*************************************************/
	else if (!strcmp(cmd, "fg"))
	{
  		if(num_arg > 1)
		{	
			illegal_cmd = TRUE ;
		}
		else if(num_arg == 0) //cmd fg without params refer to last job from bg.
		{
			Pjob job_elem = get_last_job(jobs) ;
			if(NULL == job_elem)
			{
				perror("smash error: > bg job – there is not bg jobs/n") ;
				return 0; // dont do notthing return success
			}
			else
			{ 
				job_pid = job_elem->pid ;
				send_job_to_fg(jobs , job_pid);
				if(kill(job_pid , SIGCONT) == 0) 
				{
					printf("smash > signal SIGCONT was sent to pid %d/n" , job_pid);
					remove_job(jobs ,job_pid);
					return 0 ; 
				}
			}
		}	
		else   //num_arg == 2
		{
			job_num = atoi(args [1]) ;
			
			if ( (job_num > (jobs->num_of_jobs)) || job_num < 1)
			{
				perror("smash error: > fg job – job does not exist") ;
				return 0 ; // dont do notthing return success
			}
			Pjob job = find_job_by_idx(jobs, job_num);   //getting the relevent job for the cmd
			printf("%s" , job->job_name);
			job_pid = job->pid ;
			
			if(kill(job_pid , SIGCONT) == 0)
			{
				printf("smash > signal SIGCONT was sent to pid %d" , job_pid);
				waitpid ( job_pid, NULL, WUNTRACED);  // todo: change it according to external 
				remove_job(jobs, job_pid);
			}
		}
		
	}
	/*************************************************/
	else if (!strcmp(cmd, "bg")) 
	{
		if (num_arg > 1 )
		{	
			illegal_cmd = TRUE ;
		} 
		else if(num_arg == 0) //cmd fg without params refer to last job that suspended .
		{
			Pjob job = get_last_suspended(jobs) ;
			if(NULL == job)
			{
				perror("smash error: > bg job – there is not suspended jobs") ;
				return 1;
			}
			else
			{
				job_pid = job->pid ;
				send_job_to_bg(jobs , job_pid);
				if(kill(job_pid , SIGCONT) == 0)
				{
					printf("smash > signal SIGCONT was sent to pid %d" , job_pid); 
					remove_job(jobs, job_pid);
					return 0 ; 
				}
				else 
				{
					printf("signal_failed ");
				}
			}
		}

		else   //num_arg == 2
		{
			job_num = atoi(args [1]) ;
			
			if ( (job_num > (jobs -> num_of_jobs)) || job_num < 1)   // verify that job index exist ; 
			{
				perror("smash error: > bg job – job does not exist") ;
				return 1 ; 
			}
			Pjob job_elem = find_job_by_idx(jobs , job_num);   //getting the relevent job for the cmd
			printf("%s" , job_elem->job_name);
			job_pid = job_elem->pid ;
			
			if(kill(job_pid , SIGCONT) == 0)
			{
				printf("smash > signal SIGCONT was sent to pid %d" , job_pid);
				remove_job(jobs,  job_pid);
				return 0 ; 
			}
			else
			{
				printf("signal failed for debug");
			}
		}
	}

	/*************************************************/
	else if (!strcmp(cmd, "quit"))
	{
		if (num_arg > 1)  
			{	
				illegal_cmd = TRUE ;
			}
		else if (num_arg == 0)  // "cmd quit" exit the smash . 
			{
				exit(0) ; 
			}	
		else if (!strcmp(args[1], "kill"))
			{	/*do for all the jobs in the list , try to SIGTERM every job, if failed check again
				for success after 5 sec then if SIGTERM failed send SIGKILL and remove job from list */
				for( int i=1 ; i <= (jobs -> num_of_jobs)  ; i ++) 
				{
						Pjob job_node = find_job_by_idx(jobs,i) ;
						job_pid = job_node -> pid  ;
						if (kill (job_pid, SIGTERM) == 0)  // SIGTERM = 15 ? //success to SIGTERM the pid
						{	
							remove_job(jobs, job_pid);
						}
						else 
						{
							sleep(5); //need-to : this is llegal command ? 
							if (kill (job_pid, SIGTERM) != 0) //check after 5 sec to SIGTERM SUCESS again
							{
								kill (job_pid, SIGKILL); //SIGKILL = 9 ? 
							}
							remove_job(jobs , job_pid);
						}		
				}					
			}				
	} 
	/*************************************************/
	else if (!strcmp(cmd, "kill"))
	{
		if (num_arg != 1)  
		{	
			illegal_cmd = TRUE ;
		}
		else
		{	
			//converting the signal number exclude the "-" to int // same for the job number; 
			int sig_num = atoi(strtok(args [1], "-" )) ; 
			if(DEBUG)printf ("\nsignum is %d ***command.c-kill*** debug\n",sig_num); //debug
			job_num = atoi(args [1]) ;
			
			if ( (job_num > (jobs -> num_of_jobs)) || job_num < 1)       //need-to : verify num_of_jobs method in "jobs" list
			{
				perror("smash error: > kill job – job does not exist") ;
				return 1 ; 
			}
			Pjob job_node = find_job_by_idx(jobs,job_num) ;   // todo: this syntax is right ?!
			job_pid = job_node -> pid ;
			
			if ( kill (job_pid, sig_num) == -1 ) // kill return 0 for success , -1 for failure ; 
			{
				perror ("smash error: > kill job – cannot send signal");
				return 1;
			}
			remove_job(jobs, job_pid) ; // remove the job from the list;
			return 0 ; 	
		}
   		
	} 
	/*************************************************/
	else // external command
	{	 		
 		ExeExternal(args, cmdString);
	 	return 0;
	}
	if (illegal_cmd == TRUE)
	{
		fprintf(stderr,"smash error: >\%s\n",cmdString);
		return 1;
	}
    return 0;
}
//**************************************************************************************
// function name: ExeExternal
// Description: executes external command
// Parameters: external command arguments, external command string
// Returns: void
//**************************************************************************************
void ExeExternal(char *args[MAX_ARG], char* cmdString) // todo: add the job to args
{
    int status; 
	int pid;
	//if(DEBUG)printf("external **** debug\n"); //debug
    	switch(pid = fork()) 
	{
    		case -1: 
					perror("smash error: > fork failed");
					break;
        	case 0 :
                	// Child Process
               		setpgrp();
					execvp(args[0], args);  // execute
					perror("smash error: > execvp failed");
                    exit(1);
					break;
			        
			default:
                if(!kill(pid,0)) add_job(jobs, cmdString, pid, ACTIVE);               
                waitpid(pid, &status, WUNTRACED);
                

	}
}
//**************************************************************************************
// function name: ExeComp
// Description: executes complicated command
// Parameters: command string
// Returns: 0- if complicated -1- if not
//**************************************************************************************
int ExeComp(Plist jobs,char* lineSize)
{
	//history_idx = (history_idx + 1) % max_history ;
	//history[max_args][history_idx] = lineSize;  //verify that line size contain the cmdString
    //strcpy(&history[0][history_idx],lineSize);

	//char ExtCmd[MAX_LINE_SIZE+2];
	char *args[MAX_ARG];
	int pid = 0;
    if ((strstr(lineSize, "|")) || (strstr(lineSize, "<")) || (strstr(lineSize, ">")) 
								|| (strstr(lineSize, "*")) || (strstr(lineSize, "?")) 
								|| (strstr(lineSize, ">>")) || (strstr(lineSize, "|&")))
    {
    	if(DEBUG)printf("comp **** debug\n"); //debug
		switch (pid = fork())
		{
			case -1 : 
				perror("fork failed");
				//todo : return? exit ? what should i do ? 
				return -1;
				break;
			case 0 : //child process 
				setpgrp(); 
				args [ 0 ] = "csh";
				args [ 1 ] = "-f";
				args [ 2 ] = "-c";
				args [ 3 ] = lineSize;
				args [ 4 ] = "/0"; // NULL
				
				/*execute the command using the c shell
				-p Uses the PATH environment variable to find the file named in the file argument to be executed 
				--execvp return value is -1 and errno just in case of failure ,never return in success*/
				execvp (args[0], args);
				
				//only if exec failed do this 
				perror("smash error: > failed to execute by execvp\n");
				//todo : return? exit ? what should i do ? 
				break;
			
			//todo:
			/*if i understand this right in the default in need to add the job for the jobs list
			the new job ruuning in the fg with the current pid from the pid = fork()
			*/
			default :  
				add_job(jobs, lineSize, pid, ACTIVE) ; //todo: who need to enter this to jobs list ? i need to do add_job ?
				waitpid(pid,NULL, WUNTRACED); 
				return 0 ;
	    }
    }
	return -1;
}
//**************************************************************************************
// function name: BgCmd
// Description: if command is in background, insert the command to jobs
// Parameters: command string, pointer to jobs
// Returns: 0- BG command -1- if not
//**************************************************************************************
int BgCmd(char* lineSize, Plist jobs)
{
	int pid = 0;
	char* cmd;
	char* delimiters = " \t\n";
	char *args[MAX_ARG];

	if (lineSize[strlen(lineSize)-2] == '&')
	{
		if(DEBUG)printf("bgcmd ****\n"); //debug
		lineSize[strlen(lineSize)-2] = '\0';
		int i = 0, num_arg = 0;
		cmd = strtok(lineSize, delimiters);
		if (NULL == cmd) {return 0;}
		args[0] = cmd;
		
		for (i=1; i<MAX_ARG; i++)
		{
			args[i] = strtok(NULL, delimiters); 
			if (args[i] != NULL) 
			num_arg++; 
		}
	
		switch (pid = fork())
		{
			case -1 :
				perror("fork failed");
				break;
				
			case 0 : //child process
				setpgrp ();
				execvp ( args [ 0 ] , args );
				perror("smash error: > failed to execute by execvp\n");	
				break;
			default :
				
				add_job(jobs, lineSize, pid, ACTIVE) ;  //without waitpid --its bg cmd
				return 0 ; 
	
		}

	}
	return -1;
}
