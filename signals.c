// signals.c
// contains signal handler funtions
// contains the function/s that set the signal handlers

/*******************************************/
/* Name: handler_cntlc
   Synopsis: handle the Control-C */
#include "jobs.h"
#include "signals.h"

extern job cur_job;
extern Plist jobs;

void handler_ctrlc() {
    if(cur_job.pid != 0){
        kill (cur_job.pid,SIGINT);
        remove_job (jobs, cur_job.pid);
        reset_job (&cur_job);    
    }
   printf("\n"); 
}

void handler_ctrlz() {
    int fg_job_pid = get_fg_job_pid(jobs);
    if (fg_job_pid != 0){
        kill (fg_job_pid,SIGTSTP);
        suspend_job_in_list(jobs, fg_job_pid);
    }
    printf("inside handle\n");
}




