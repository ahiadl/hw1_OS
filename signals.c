// signals.c
// contains signal handler funtions
// contains the function/s that set the signal handlers

/*******************************************/
/* Name: handler_cntlc
   Synopsis: handle the Control-C */
#include "jobs.h"
#include "signals.h"

extern Pjob cur_job;
extern Plist jobs;

void handler_ctrlc() {
    kill (cur_job->pid,SIGINT);
    remove_job (jobs, cur_job->pid);
    reset_job (cur_job);     
}

void handler_ctrlz() {
    kill (cur_job->pid,SIGTSTP);
    cur_job->suspended = SUSPENDED;
    add_job(jobs, cur_job->name, cur_job->pid, cur_job->suspended);
}

