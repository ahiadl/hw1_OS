#include "jobs.h"
#include "stdio.h"
#include "string.h"
#include "stdbool.h"
#include "stdlib.h"

Pjob find_job_by_idx(Plist list_inst, int idx){
    if (list_inst->first_node = NULL) return;
    nil nodes; 
    nodes.cur_node = list_inst->first_node;
    nodes.prev_node = NULL;
    nodes.next_node = nodes.cur_node->next;
    int found = (idx == nodes.cur_node->job_elem->idx);
    while(!found && (nodes.cur_node != NULL)) {
        nodes.prev_node = nodes.cur_node;
        nodes.cur_node  = nodes.cur_node->next;
        nodes.next_node = nodes.cur_node->next;
        found = (idx == nodes.cur_node->job_elem->idx);
    }
    if (nodes.cur_node == NULL) return NULL;
    else return nodes.cur_node->job_elem;
}

Pjob find_job_by_pid(Plist list_inst, int pid){
    if (list_inst->first_node = NULL) return;
    nil nodes; 
    nodes.cur_node = list_inst->first_node;
    nodes.prev_node = NULL;
    nodes.next_node = nodes.cur_node->next;
    int found = (pid == nodes.cur_node->job_elem->pid);     
    while(!found && (nodes.cur_node != NULL)) {
        nodes.prev_node = nodes.cur_node;
        nodes.cur_node  = nodes.cur_node->next;
        nodes.next_node = nodes.cur_node->next;
        found = (pid == nodes.cur_node->job_elem->pid);    
    }
    if (nodes.cur_node == NULL) return NULL;
    else return nodes.cur_node->job_elem;
}

Pnil find_node(Plist list_inst, int pid){
    if (list_inst->first_node = NULL) return;
    nil nodes; 
    nodes.cur_node = list_inst->first_node;
    nodes.prev_node = NULL;
    nodes.next_node = nodes.cur_node->next;
    int found = (pid == nodes.cur_node->job_elem->pid);
    while(!found && (nodes.cur_node != NULL)) {
        nodes.prev_node = nodes.cur_node;
        nodes.cur_node  = nodes.cur_node->next;
        nodes.next_node = nodes.cur_node->next;
        found = (pid == nodes.cur_node->job_elem->pid);
    }
    if (nodes.cur_node == NULL) return NULL;
    else return nodes;
}

void reset_job (Pjob job_inst){
    job_temp->name = "\0" ;        
    job_temp->pid = 0;
    job_temp->idx = 0;
    job_temp->suspended = 0;
    job_temp->start_time = 0;    
}

Pjob init_job (int name_length){
    Pjob job_temp = (Pjob)malloc(sizof(job));
    char* name = (char*)malloc(name_length*sizeof(char));
    reset_job(job_temp); 
    return job_temp;
}

Pjob init_node (){
    Pnode node_temp = (Pnode)malloc(sizof(node));
    node_temp->next = NULL;
    node_temp->job_elem = NULL;
    return node_temp;
}

fill_job_params(Pjob new_job, char* name, int pid, int idx, int suspended, int time, int update_time){
    new_job->pid = pid;
    new_job->suspended = suspended;
    new_job->idx = idx;
    strcpy(new_job->name, name);
    if (UPDATE == update_time) new_job->start_time = time();
    else new_job->start_time=time;
}

void add_job(Plist list_inst, char* name, int pid, int suspended){
    //allocating dynamic memory 
    Pnode new_node = init_node();
    Pjob new_job = init_job(strlen(name));
    //positioning and linking
    new_node->next_node = NULL;
    new_node->job_elem = new_job;
    list_inst->last_node->next_node = new_node;
    list_inst->last_node = new_node;
    //fill params 
    fill_job_params(new_job, name, pid, linst_inst-.num_of_jobs+1,suspended,0,UPDATE);
}

void align_idx(Plist list_inst){
    if (list_inst->first_node == NULL) return;           
    Pnode cur_node = jobs->first_node;
    int i = 0;
    do {
        cur_node->job->idx = i;
        cur_node = cur_node->next;
        i++;
    }
    while(cur_node != NULL) 
}

int remove_job(Plist list_inst, int pid){ //TODO: check if pid correct
    nil rm_node = find_node(list_inst, pid);
    nil.prev_node->next=nil.next_node;
    free(nil.cur_node->job->name);
    free(nil.cur_node->job);
    free(nil.cur_node);
    align_idx(list_inst);
    list_inst->num_of_jobs--;
}

Pjob get_last_suspended(Plist list_inst){
    if (list->first_node == NULL) return NULL;
    int idx = list_inst->num_of_jobs;
    Pjob cur_job;
    int found = 0;
    while (found == 0 || idx > 0){ //TODO: think again of stop conditions
        cur_job = find_job_by_idx(list_inst, idx)
        found = (SUSPENDED == cur_node->job->suspended);
        idx--;
    }
    if (idx == 0 ) return NULL;
    else return cur_job;
}

Pjob get_last_job (Plist list_inst){
    return find_job_by_idx (list_inst->last_node);
}

void print_jobs(jobs_list* list_inst){
    int idx = 1;
    Pjob cur_job;
    while (idx>list_inst->num_of_jobs){
        cur_job = find_job_by_idx(list, idx);
        if (SUSPENDED == cur_job->job_suspended) printf ("[%d] %s %d %d secs (Stopped)\n",cur_job->idx, cur_job->name, cur_job->pid, time()-cur_job->start_time);
        else printf ("[%d] %s %d %d secs\n",cur_job->idx, cur_job->name, cur_job->pid, time()-cur_job->start_time);
        idx++;
    }
}

Plist init_list(){
    Plist list_temp = (Plist)malloc(sizeif(list));
    list_temp->num_of_jobs = 0;
    list_temp->first_node  = NULL;
    list_temp->last_node   = NULL;
    return ; 
}

Void destroy_list(Plist list_inst){
    int i = 1;
    int pid;
    Pjob cur_job;
    while (i=< list_inst->num_of_jobs){
        cur_job = find_job_by_idx(list_inst, i);
        remove_job(list_inst, cur_job->pid);
    }
    free (list_inst);
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
