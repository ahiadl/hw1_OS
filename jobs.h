#include <stdio.h>
#include <string.h>

typedef struct {
    char* job_name;
    int pid;
    int start_time;
    int suspended;
    int idx;
}job, *Pjob;

typedef struct {
    node* next;
    job_elem* job;
}node, *Pnode;

typedef struct {
    node* first_node;
    node* last_node;
    int num_of_jobs;
}list,*Plist;
 
enum {
   GET,
   SET,
};

enum {
    FAILURE =  0,
    SUCCESS = -1,
};

typedef struct {
    Pnode prev_node;
    Pnode cur_node;
    Pnode next_node;
}nil, Pnil; //nil = node in list

enum{
    ACTIVE    = 0,
    SUSPENDED = 1,
};

void print_jobs(jobs_list* list);
Pjob get_last_suspended (jpbs_list* jobs);
Pjob find_job_by_idx(jobs_list* jobs);
Pjob find_job_by_pid(jobs_list *jobs);
int remove_job (jobs_list* jobs, int pid);
int add_job (job_list* jobs, string name, int pid, int time);
Plist init_list ();
