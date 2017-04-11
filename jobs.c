#include "jobs.h"
#include "stdio.h"
#include "string.h"
#include "stdbool.h"
#include "stdlib.h"


Pjob find_job_by_idx(Plist jobs, int idx){
    if (jobs->first_node = NULL) return;
    nil nodes; 
    nodes.cur_node = jobs->first_node;
    nodes.prev_node = NULL;
    nodes.next_node = nodes.cur_node->next;
    int found = (idx == nodes.cur_node->job->idx);
    while(!found && (nodes.cur_node != NULL)) {
        nodes.prev_node = nodes.cur_node;
        nodes.cur_node  = nodes.cur_node->next;
        nodes.next_node = nodes.cur_node->next;
        found = (idx == nodes.cur_node->job->idx);
    }
    if (nodes.cur_node == NULL) return NULL;
    else return nodes.cur_node->job;
}

Pjob find_job_by_pid(Plist jobs, int pid){
    if (jobs->first_node = NULL) return;
    nil nodes; 
    nodes.cur_node = jobs->first_node;
    nodes.prev_node = NULL;
    nodes.next_node = nodes.cur_node->next;
    int found = (pid == nodes.cur_node->job->pid);     
    while(!found && (nodes.cur_node != NULL)) {
        nodes.prev_node = nodes.cur_node;
        nodes.cur_node  = nodes.cur_node->next;
        nodes.next_node = nodes.cur_node->next;
        found = (pid == nodes.cur_node->job->pid);    
    }
    if (nodes.cur_node == NULL) return NULL;
    else return nodes.cur_node->job;
}

Pnil find_node(Plist jobs, int pid){
    if (jobs->first_node = NULL) return;
    nil nodes; 
    nodes.cur_node = jobs->first_node;
    nodes.prev_node = NULL;
    nodes.next_node = nodes.cur_node->next;
    int found = (pid == nodes.cur_node->job->pid);
    while(!found && (nodes.cur_node != NULL)) {
        nodes.prev_node = nodes.cur_node;
        nodes.cur_node  = nodes.cur_node->next;
        nodes.next_node = nodes.cur_node->next;
        found = (pid == nodes.cur_node->job->pid);
    }
    if (nodes.cur_node == NULL) return NULL;
    else return nodes;
}

void add_job(Plist list, char* name, int pid){
    //allocating dynamic memory 
    struct node* new_node = (struct node*)malloc(sizeof(node));
    struct job_elem* new_job = (struct job_elem*)malloc(sizeof(job_elem));
    //positioning and linking
    new_node->next_node = NULL;
    new_node->job = new_job;
    list->last_node->next_node = new_node;
    list->last_node->new_node;
    //fill params 
    new_job->pid = pid;
    new_job->suspended = ACTIVE;
    new_job->idx = (jobs->num_of_jobs) + 1;
    strcpy(new_job->name, name);
    new_job->start_time = time();
    list->num_of_jobs = new_job->idx;
}

void align_idx(Plist list){
    if (list->first_node == NULL) return;           
    Pnode cur_node = jobs->first_node;
    int i = 0;
    do {
    cur_node->job->idx = i;
    cur_node = cur_node->next;
    i++;
    }
    while(cur_node != NULL) 
}

int remove_job(Plist list, int pid){
    nil rm_node = find_node(list, pid);
    nil.prev_node->next=nil.next_node;
    free(nil.cur_node->job);
    free(nil.cur_node);
    align_idx(Plist list);
    list->num_of_jobs--;
}

Pjob get_last_suspended(Plist list){
    if (list->first_node == NULL) return NULL;
    int idx = list->num_of_jobs;
    Pjob cur_job;
    int found = 0;
    while (found == 0 || idx > 0){ //TODO: think again of stop conditions
        cur_job = find_job_by_idx(list, idx)
        found = (SUSPENDED == cur_node->job->suspended);
        idx--;
    }
    if (idx == 0 ) return NULL;
    else return cur_job;
}

void print_jobs(jobs_list* list){
    int idx = 1;
    Pjob cur_job;
    while (idx>list->num_of_jobs){
        cur_job = find_job_by_idx(list, idx);
        if (SUSPENDED == cur_job->job_suspended) printf ("[%d] %s %d %d secs (Stopped)\n",cur_job->idx, cur_job->name, cur_job->pid, time()-cur_job->start_time);
        else printf ("[%d] %s %d %d secs\n",cur_job->idx, cur_job->name, cur_job->pid, time()-cur_job->start_time);
        idx++;
    }
}

Plist init_list(){
    Plist list = (Plist) malloc (sizeof(list));
    list->num_of_jobs = 0;
    list->first_node  = NULL;
    list->last_node   = NULL;
    return list; 
}




/*
int set_name (job_elem* job,  jobs_str* jobs, string job_name, int get_set_,string value){
        job_elem job = find_job (jobs_str* jobs, string job_name);
        if(get_set_ = GET){
            return job->start_time;
        }else if (get_set_ = SET){
           job->start_time = value;
           return SUCCESS;
        }
        return FAILURE;
}*/

/*
void reset_job (Pjob_elem job){
    job->pid = 0;
    job->start_time = 0; 
    job->name = "/0";
    job->
}*/
/*
void reset_node (Pnode list_node){
    list_node->next = NULL;
    reset_job (list_node->job);
}*/
/*int set_get_pid (jobs_str* jobs, string job_name, int get_set_,int value){
    job_elem job = find_job (jobs_str* jobs, string job_name);
    if( get_set_ = GET){
        return job->pid;
    } else if (get_set_ = SET){
        job->pid = value;
        return SUCCESS;
    }
    return FAILURE;
}*/
/*int set_get_start_time (job_elem* job,  jobs_str* jobs, string job_name, int get_set_,int value){
        job_elem job = find_job (jobs_str* jobs, string job_name);
        if(get_set_ = GET){
            return job->start_time;
        }else if (get_set_ = SET){
           job->start_time = value;
           return SUCCESS;
        }
        return FAILURE;
}*/
