// signals.c
// contains signal handler funtions
// contains the function/s that set the signal handlers

/*******************************************/
/* Name: handler_cntlc
   Synopsis: handle the Control-C */
#include "jobs.h"
#include "signals.h"

extern Plist jobs;
extern job fg_job;

void handler_ctrlc() {
    int fg_job_pid = get_fg_job_pid(jobs);
    if (fg_job_pid != 0){
        printf ("\nfg job pid :%d\n",fg_job_pid);
        int status = kill (fg_job_pid, SIGINT);
        if (!status) remove_job(jobs, fg_job_pid);
    }
    if(DEBUG) printf("\ninside ctrlc handle\n");

}

void handler_ctrlz() {
    int fg_job_pid = get_fg_job_pid(jobs);
    if (fg_job_pid != 0){
        printf ("\nfg job pid :%d\n",fg_job_pid);
        int status = kill(fg_job_pid, SIGTSTP);
        printf("status: %d\n", status);
        if(status != 0) printf("failed to send signal  sigstp\n");
        else{
            send_job_to_bg(jobs, fg_job_pid);
            suspend_job_in_list(jobs, fg_job_pid, SUSPENDED);
        }
    }
    if(DEBUG) printf("\ninside ctrlz handle\n");
}

void handler_chld(){
    int jobs_num = jobs->num_of_jobs;
    int cur_job_idx = 0;
    int return_pid;
    int status;
    Pjob cur_job;
    if(DEBUG)printf("inside child signal handler\n");
    for (cur_job_idx = 1; cur_job_idx <= jobs_num; cur_job_idx++){
         cur_job = find_job_by_idx(jobs, cur_job_idx);
         if (cur_job == NULL) break;
         //if(DEBUG) print_job(cur_job);
         if(0 != kill(cur_job->pid,0)){
             if(DEBUG)printf("process %d was removed by kill indication signal \n", cur_job->pid);
             remove_job(jobs, cur_job->pid);                                                                              
         }
         return_pid = waitpid (cur_job->pid, &status, WNOHANG|WUNTRACED|WCONTINUED);     
         /******************check what happened to child process*********************/
        /* if      (WIFSTOPPED(status)){                                                   
             if(DEBUG) printf("Stopped Child \n");                                       
             suspend_job_in_list(jobs, cur_job->pid, SUSPENDED);                         
         }                                                                               
         else if (WIFCONTINUED(status)){                                                 
            if(DEBUG) printf("Stopped Continued \n");                                    
             suspend_job_in_list(jobs, cur_job->pid, ACTIVE);                            
         }                                                                               
         else*/ if (WIFEXITED(status)){                                                    
            switch(return_pid){                                                          
               case -1 :
                   if(DEBUG) printf("process %d returned error",cur_job->pid);                          
                   break;
               case  0 :                                                                  
                   if(DEBUG) printf("process %d is still running\n",cur_job->pid);      
                   break;                                                               
               default:                                                                 
                   if (DEBUG) printf("removing process: %d from list", cur_job->pid);   
                   remove_job(jobs, cur_job->pid);                                      
                   cur_job_idx--;                                                       
                   jobs_num = jobs->num_of_jobs;                                        
                   break;                                                               
            }                                                                            
         }
    }
}





























