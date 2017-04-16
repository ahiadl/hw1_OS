//		commands.c
//********************************************
#include "commands.h"
#define max_history 50

//global varibles : // need-to : maybe it's better to pass it as argument to ExeCmd ? 
int last_suspended_pid;  
extern Pjob cur_job; 

//********************************************
// function name: ExeCmd
// Description: interperts and executes built-in commands
// Parameters: pointer to jobs, command string
// Returns: 0 - success,1 - failure
//**************************************************************************************
int ExeCmd(void* jobs, char* lineSize, char* cmdString) //one cmd per time
{	
	history_idx = 0 ;                   //TODO: solve this implementation problem.  
	char history[max_history] = {null}; // 
	char cd_prev = null;  
	int last_bg_pid = NULL ;
	int last_suspended_pid = NULL ;
	int job_pid = null ; 
	int job_num = null ;
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
	

//todo: check about return values for errors 
//need-to : verify all jobs (list_type) methods.

//list method used: 
//total_jobs - the number of all the jobs in the list.
//job_num - needed index of the jobs in the list [1 , total_jobs]  -- starting by 1 !! 
//cmd_name - for example "b.out"    -- (they call it command_number)
//job_pid - string of the job pid;
//suspended - indicate if the process is suspended.

/*print_jobs(&jobs) - print all the jobs by the requsted format. -- return value void ? */

/*get_last_suspended(&jobs) - return the node of relevent node ; 

/*find_job_by_idx(&jobs,job_num) - finding job by given job_num (implemented by iterators "for"); 
 -- return value is the ptr to node of the job and its allow to get the job_pid
 -- example for use: 
jobs *target_job_for_kill = find_job_by_idx(&jobs,job_num) ;
int  pid_to_kill = target_job_for_kill -> job_pid ; */

/*find_job_by_pid(&jobs,job_pid) - just like the function above but finding the node by pid */

/*remove_job(&spesofic_job_node) - remove job from the list - return void ? */


 
 /*************************************************/
	//if not history cmd do this -- history shouldn't added and printed in the history list 
	if (strcmp(cmd, "history")) 
	{
		history_idx = (history_idx + 1) % max_history ; 
		history[history_idx] = cmd;   // todo: verify that all the command string taken 
	}
	
	if (!strcmp(cmd, "cd") ) 
	{	
		if (num_arg != 2)  
		{	
			illegal_cmd = TRUE ;
		}
		else
		{			
			if (!strcmp(args[2], "-"))   	      	   //change to prev directiry
			{	
				if(cd_prev != null)
					temp_dir = getcwd(pwd, MAX_LINE_SIZE);
					chdir(cd_prev);
					cd_prev = temp_dir ;
			}
			else  					
			    if(!(chdir(args[2]))) 			// chdir return 0 for success //need-to see error code ? 
				    perror("smash error: > “path” - path not found");
			    else
				    cd_prev = cmd[2];
				
		}	
			
	} 
	
	/*************************************************/
	else if (!strcmp(cmd, "pwd")) 
	{
		if(num_arg !=1)
		{
			illegal_cmd = TRUE ;  //illegal input
		}
		else 
		{	
			getcwd(pwd, MAX_LINE_SIZE);
			printf("%s\n", pwd);
		}
		
	}
    /*************************************************/
	else if (!strcmp(cmd, "jobs"))  //this function print 2D array ? no changed to list type
	{
 		if(num_arg !=1)
		{
			illegal_cmd = TRUE ;  //illegal input
		}
		else 
		{	
			print_jobs(&jobs);
		}
	}
	/*************************************************/
	else if (!strcmp(cmd, "showpid")) 
	{
		if(num_arg !=1)
		{
			illegal_cmd = TRUE ; //illegal input
		}
		else 
		{
			job_pid = getpid();    //todo: print the relevent pid , take the pid from jobs.
			printf("Smash pid is %d\n", pid);
		}
	}
	/*************************************************/
	else if (!strcmp(cmd, "history")) 
	{
		if(num_arg !=1)
		{
			illegal_cmd = TRUE ; //illegal input
		}
		else 
		{	//running 50 time print only if not NULL // FIFO Printing 
			for(i=history_idx ; i < (max_history + history_idx) ; i++)  
			{
			if(NULL != history[i+1])
				{
					printf("%s\n", history[i+1]);
				}
			}
			
		}
	} 
	/*************************************************/
	else if (!strcmp(cmd, "fg")) 
	{
  		if (num_arg > 2 )  
		{	
			illegal_cmd = TRUE ;
		}
		else if(num_arg == 1) //cmd fg without params refer to last job from bg.
		{
            job 
            if(null == last_bg_pid)
			{
				perror("smash error: > bg job – there is not bg jobs") ;
				return 0 // dont do notthing return success
			}
			else
			{
				Pjob job_search = find_job_by_pid(&jobs , last_bg_pid);
				job_pid = job_search -> job_pid ;
				if(kill(job_pid , SIGCONT) == 0)
				{
					printf("smash > signal SIGCONT was sent to pid %d" , job_pid);
					remove_job(&node); 
					return 0 ; 
				}
			}
		}	
		else   //num_arg == 2
		{
			job_num = atoi(args [2]) ;
			
			if ( (job_num > (jobs -> total_jobs)) || job_num < 1)   // verify that job index exist ; 
			{
				perror("smash error: > fg job – job does not exist") ;
				return 0 ; // dont do notthing return success
			}
			Pjob job_temp = find_job_by_idx(&jobs , job_num);   //getting the relevent job for the cmd
			printf("%s" , node->job_name);
			job_pid = atoi(node -> job_pid) ; 
			
			if(kill(job_pid , SIGCONT) == 0)
			{
				printf("smash > signal SIGCONT was sent to pid %d" , job_pid);
				waitpid ( job_pid, NULL, 0 );  //see waitpid manual for more info ; //need-to: this is right ? 
				remove_job(&node); 
			}
		}
		
	}
	/*************************************************/
	else if (!strcmp(cmd, "bg")) 
	if (num_arg > 2 )  
		{	
			illegal_cmd = TRUE ;
		} 
		else if(num_arg == 1) //cmd fg without params refer to last job that suspended .
		{
			if(null == last_suspended_pid)
			{
				perror("smash error: > bg job – there is not suspended jobs") ;
				return 1 
			}
			else
			{
				jobs *node = get_last_suspended(&jobs) ;
				if(kill(job_pid , SIGCONT) == 0)
				{
					printf("smash > signal SIGCONT was sent to pid %d" , job_pid);
					last_bg_pid = job_pid ; 
					remove_job(&node); 
					return 0 ; 
				}
			}
			
		}	
		else   //num_arg == 2
		{
			job_num = atoi(args [2]) ;
			
			if ( (job_num > (jobs -> total_jobs)) || job_num < 1)   // verify that job index exist ; 
			{
				perror("smash error: > bg job – job does not exist") ;
				return 1 ; 
			}
			jobs *node = find_job_by_idx(&jobs , job_num);   //getting the relevent job for the cmd
			printf("%s" , node->job_name);
			job_pid = node->job_pid ; 
			
			if(kill(job_pid , SIGCONT) == 0)
			{
				printf("smash > signal SIGCONT was sent to pid %d" , job_pid);
				remove_job(&node); 
				return 0 ; 
			}
		}
		
	}
	/*************************************************/
	else if (!strcmp(cmd, "quit"))
	{
		if (num_arg > 2)  
			{	
				illegal_cmd = TRUE ;
			}
		else if (num_arg == 1)  // "cmd quit" exit the smash . 
			{
				exit(0) ; 
			}	
		else if (!strcmp(args[2], "kill"))
			{	/*do for all the jobs in the list , try to SIGTERM every job, if failed check again
				for success after 5 sec then if SIGTERM failed send SIGKILL and remove job from list */
				for( int i=1 ; i <= (jobs -> total_jobs)  ; i ++) 
				{
						jobs *job_node = find_job_by_idx(&jobs,i) ;   
						job_pid = job_node -> job_pid 
						if (kill (job_pid, SIGTERM) == 0)  // SIGTERM = 15 ? //success to SIGTERM the pid
						{	
							remove_job(&job_node); 
						}
						else 
						{
							sleep(5); //need-to : this is llegal command ? 
							if (kill (job_pid, SIGTERM) != 0) //check after 5 sec to SIGTERM SUCESS again
							{
								kill (job_pid, SIGKILL); //SIGKILL = 9 ? 
							}
							remove_job(&job_node);     
						}		
				}					
			}				
	} 
	/*************************************************/
	else if (!strcmp(cmd, "kill"))
	{
		if (num_arg != 2)  
		{	
			illegal_cmd = TRUE ;
		}
		else
		{	//atoi convert string to int
			//converting the signal number exclude the "-" to int // same for the job number; 
			int sig_num = atoi(strtok(args [1], "-" )) ; 
			job_num = atoi(args [2]) ;
			
			if ( (job_num > (jobs -> total_jobs)) || job_num < 1)       //need-to : verify total_jobs method in "jobs" list
			{
				perror("smash error: > kill job – job does not exist") ;
				return 1 ; 
			}
			jobs *job_node = find_job_by_idx(&jobs,job_num) ;   // todo: this syntax is right ?!	
			job_pid = atoi(job_node -> job_pid) ;
			
			if ( kill (job_pid, sig_num) == -1 ) // kill return 0 for success , -1 for failure ; 
			{
				perror ("smash error: > kill job – cannot send signal");
				return 1;
			}
			remove_job(&job_node) ; // remove the job from the list; 
			return 0 ; 	
		}
   		
	} 
	/*************************************************/
	else if (!bgcmd)// external command
	{
 		ExeExternal(args, cmdString);
	 	return 0;
	}
	if (illegal_cmd == TRUE)
	{
		perror("smash error: > \"%s\"\n", cmdString);
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
void ExeExternal(char *args[MAX_ARG], char* cmdString)
{
	int pID;
    	switch(pID = fork()) 
	    {
    		case -1: 
					// Add your code here (error)
			        exit();
                    break;
        	case 0 :
                	// Child Process
               		setpgrp();
                    exec(" ",cmdString);
                    break;
			default:
                    int status;
                    //TODO: insert process to fg job;
                    waitpid(pID, &status, (WUNTRACED | WNOHANG));
                    if (TRUE == WIFEXITED(status)) reset_job(cur_job); //if process ended normally change fg job to null
                    break;
	    }
}
//**************************************************************************************
// function name: ExeComp
// Description: executes complicated command
// Parameters: command string
// Returns: 0- if complicated -1- if not
//**************************************************************************************
int ExeComp(char* lineSize)
{
	char ExtCmd[MAX_LINE_SIZE+2];
	char *args[MAX_ARG];
    if ((strstr(lineSize, "|")) || (strstr(lineSize, "<")) || (strstr(lineSize, ">")) 
								|| (strstr(lineSize, "*")) || (strstr(lineSize, "?")) 
								|| (strstr(lineSize, ">>")) || (strstr(lineSize, "|&")))
    {
		// Add your code here (execute a complicated command)
					
		/* 
		your code
		*/
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

	char* Command;
	char* delimiters = " \t\n";
	char *args[MAX_ARG];
    bgcmd = 1;
	if (lineSize[strlen(lineSize)-2] == '&')
	{
		lineSize[strlen(lineSize)-2] = '\0';
        
        
        int pID;
        switch(pID = fork())
        {
            case -1:
                // Add your code here (error)
                exit();
                break;
            case 0 :
                // Child Process
                setpgrp();
                if(!ExeComp()) exit();
                if(!exeCmd()) exit;
                exec(cmd);
                break;
            default:
                int status;
                fill_job_params(cur_job, cmdString, pID, jobs->num_of_jobs+1, 0, DONT_UPDATE);
                if (TRUE == WIFEXITED(status)) reset_job(cur_job); //if process ended normally change fg job to null
                break;
        }
	}
	return -1;
}

