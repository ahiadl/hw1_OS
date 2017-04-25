#include "jobs.h"
#include "stdio.h"
#include "string.h"
#include "stdbool.h"
#include "stdlib.h"

nil nil_global;

Pjob find_job_by_idx(Plist list_inst, int idx){
    printf("inside find job by idx: %d\n", idx);
    if (list_inst->first_node == NULL) return NULL;
    printf("we have some jobs to scan\n");
    nil nodes; 
    nodes.cur_node = list_inst->first_node;
    nodes.prev_node = NULL;
    nodes.next_node = nodes.cur_node->next;
    int found = (idx == nodes.cur_node->job_elem->idx);
    printf("first has %d found\n", found);
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
    if (list_inst->first_node == NULL) return NULL;
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

nil find_node(Plist list_inst, int pid){
    nil nodes;
    if (list_inst->first_node == NULL){
        nodes.cur_node  = NULL;
        nodes.prev_node = NULL;
        nodes.next_node = NULL;
        return nodes;
    }else{
        nodes.cur_node  = list_inst->first_node;
        nodes.prev_node = NULL;
        nodes.next_node = nodes.cur_node->next;
        int found = (pid == nodes.cur_node->job_elem->pid);
        while(!found && (nodes.cur_node != NULL)) {
            nodes.prev_node = nodes.cur_node;
            nodes.cur_node  = nodes.cur_node->next;
            nodes.next_node = nodes.cur_node->next;
            found = (pid == nodes.cur_node->job_elem->pid);
        }
        if (nodes.cur_node == NULL){
           nodes.cur_node  = NULL;
           nodes.prev_node = NULL;
           nodes.next_node = NULL;
        }
        return nodes;
    }
}

void reset_job (Pjob job_inst){
    job_inst->job_name = "\0" ;        
    job_inst->pid = 0;
    job_inst->idx = 0;
    job_inst->suspended = 0;
    job_inst->start_time = 0;   
    job_inst->bg_fg = FG; 
}

Pjob init_job (int name_length){
    Pjob job_temp = (Pjob)malloc(sizeof(job));
    char* name = (char*)malloc(name_length*sizeof(char));
    reset_job(job_temp);
    job_temp->job_name = name; 
    return job_temp;
}

Pnode init_node (){
    Pnode node_temp = (Pnode)malloc(sizeof(node));
    node_temp->next = NULL;
    node_temp->job_elem = NULL;
    return node_temp;
}

void fill_job_params(Pjob new_job, char* name, int pid, int idx, int suspended, int user_time, int update_time){
    new_job->pid = pid;
    new_job->suspended = suspended;
    new_job->idx = idx;
    strcpy(new_job->job_name, name);
    if (UPDATE == update_time) new_job->start_time = time(NULL);
    else new_job->start_time = user_time;
}

void add_job(Plist list_inst, char* name, int pid, int suspended){
    //allocating dynamic memory
    if(DEBUG) printf("adding job to list"); 
    Pnode new_node = init_node();
    Pjob new_job = init_job(strlen(name));
    //positioning and linking
    new_node->next = NULL;
    new_node->job_elem = new_job;
    if (list_inst->num_of_jobs == 0){
        list_inst->last_node = new_node;
        list_inst->first_node = new_node;
    }
    else list_inst->last_node->next = new_node;
    list_inst->num_of_jobs =list_inst->num_of_jobs + 1; 
    if(DEBUG)printf("done allocating memory and adding to list starting filling params\n");
    if(DEBUG)printf("params: name: %s pid: %d suspended: %d\n", name, pid, suspended);
    //fill params 
    fill_job_params(new_job, name, pid, list_inst->num_of_jobs,suspended,0,UPDATE);
}

void align_idx(Plist list_inst){
    if (list_inst->first_node == NULL) return;           
    Pnode cur_node = list_inst->first_node;
    int i = 0;
    do {
        cur_node->job_elem->idx = i;
        cur_node = cur_node->next;
        i++;
    }
    while(cur_node != NULL); 
}

int remove_job(Plist list_inst, int pid){ //TODO: check if pid correct
    nil nil_res = find_node(list_inst, pid);
    if (nil_res.cur_node == NULL) return FAILURE;
    nil_res.prev_node->next=nil_res.next_node;
    free(nil_res.cur_node->job_elem->job_name);
    free(nil_res.cur_node->job_elem);
    free(nil_res.cur_node);
    align_idx(list_inst);
    list_inst->num_of_jobs--;
    return SUCCESS;
}

Pjob get_last_suspended(Plist list_inst){
    if (list_inst->first_node == NULL) return NULL;
    int idx = list_inst->num_of_jobs;
    Pjob cur_job;
    int found = 0;
    while (found == 0 || idx > 0){ //TODO: think again of stop conditions
        cur_job = find_job_by_idx(list_inst, idx);
        found = (SUSPENDED == cur_job->suspended);
        idx--;
    }
    if (idx == 0 ) return NULL;
    else return cur_job;
}

Pjob get_last_job (Plist list_inst){
    return list_inst->last_node->job_elem;
}

void print_jobs(Plist list_inst){
    int idx = 1;
    Pjob cur_job;
    printf("inside print jobs, we have %d jobs to print\n", list_inst->num_of_jobs);
    if (list_inst->num_of_jobs == 0) return;
    while (idx <= list_inst->num_of_jobs){
        cur_job = find_job_by_idx(list_inst, idx);
        if (SUSPENDED == cur_job->suspended) printf ("[%d] %s %d %ld secs (Stopped)\n",cur_job->idx, cur_job->job_name, cur_job->pid, (time(NULL) - cur_job->start_time));
        else printf ("[%d] %s %d %ld secs\n",cur_job->idx, cur_job->job_name, cur_job->pid, (time(NULL) - cur_job->start_time));
        idx++;
    }
}

Plist init_list(){
    Plist list_inst = (Plist)malloc(sizeof(list));
    list_inst->num_of_jobs = 0;
    list_inst->first_node  = NULL;
    list_inst->last_node   = NULL;
    return list_inst;
}

void destroy_list(Plist list_inst){
    int i = list_inst->num_of_jobs;
    Pjob cur_job;
    while (i > 0){
        cur_job = find_job_by_idx(list_inst, i);
        remove_job(list_inst, cur_job->pid);
        i--;
    }
    free (list_inst);
}

void suspend_job_in_list (Plist list_inst, int pid){
    printf("inside_suspended");
    Pjob job_inst = find_job_by_pid(list_inst, pid);
    job_inst->suspended = SUSPENDED;
}

void send_job_to_bg (Plist list_inst, int pid){
    Pjob job_inst = find_job_by_pid(list_inst, pid);
    job_inst->bg_fg = BG;
}

void send_job_to_fg (Plist list_inst, int pid){
    Pjob job_inst = find_job_by_pid(list_inst, pid);
    job_inst->bg_fg = FG;
}


int get_fg_job_pid (Plist list_inst){
       if (list_inst->first_node == NULL) return 0;
       nil nodes; 
       nodes.cur_node = list_inst->first_node;
       nodes.prev_node = NULL;
       nodes.next_node = nodes.cur_node->next;
       int found = (FG == nodes.cur_node->job_elem->bg_fg);
       while(!found && (nodes.cur_node != NULL)) {
           nodes.prev_node = nodes.cur_node;
           nodes.cur_node  = nodes.cur_node->next;
           nodes.next_node = nodes.cur_node->next;
           found = (FG == nodes.cur_node->job_elem->bg_fg);
       }
       if (nodes.cur_node == NULL) return 0;
       else return nodes.cur_node->job_elem->pid;
   }
   
