#include <stdio.h>
#include <string.h>
//test

typedef struct {
    char* job_name; //TODO: dynamic allocation of pointed memory
    int pid;
    int start_time;
    int suspended;
    int idx;
}job, *Pjob;

typedef struct node{
    struct node* next;
    Pjob job_elem;
}node, *Pnode;

typedef struct {
    Pnode first_node;
    Pnode last_node;
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
}nil, *Pnil; //nil = node in list

enum{
    ACTIVE    = 0,
    SUSPENDED = 1,
};

enum{
    DONT_UPDATE,
    UPDATE,
};

Pjob find_job_by_idx(Plist list_inst, int idx);
Pjob find_job_by_pid(Plist list_inst, int pid);
nil find_node(Plist list_inst, int pid);
void reset_job (Pjob job_inst);
Pjob init_job (int name_length);
Pjob init_node ();
void add_job(Plist list_inst, char* name, int pid);
void align_idx(Plist list_inst);
int remove_job(Plist list_inst, int pid);
Pjob get_last_suspended(Plist list_inst);
Pjob get_last_job (Plist list_inst);
void print_jobs(Plist list_inst);
Plist init_list();
void destroy_list(Plist list_inst);

