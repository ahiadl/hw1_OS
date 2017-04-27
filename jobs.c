#include "jobs.h"
#include "stdio.h"
#include "string.h"
#include "stdbool.h"
#include "stdlib.h"

nil nil_global;


Pjob find_job_by_idx(Plist list_inst, int idx){
        if (list_inst->first_node == NULL) return NULL;
    nil nodes; 
    int found = 0;
    nodes.cur_node = list_inst->first_node;
    nodes.prev_node = NULL;
    nodes.next_node = nodes.cur_node->next;
    while(!found && (nodes.cur_node != NULL)) {
        //if(DEBUG) printf("start scaning loop, current job:\n");
        //if(DEBUG) print_job(nodes.cur_node->job_elem);
        
        found = (idx == nodes.cur_node->job_elem->idx);
        if (found) continue;
        nodes.prev_node = nodes.cur_node;
        nodes.cur_node  = nodes.cur_node->next;
        if(nodes.cur_node != NULL) nodes.next_node = nodes.cur_node->next;
    }
    if (nodes.cur_node == NULL) return NULL;
    else return nodes.cur_node->job_elem;
}

Pjob find_job_by_pid(Plist list_inst, int pid){
    if (list_inst->first_node == NULL) return NULL;
    nil nodes;
    int found = 0;  
    nodes.cur_node = list_inst->first_node;
    nodes.prev_node = NULL;
    nodes.next_node = nodes.cur_node->next;
    while(!found && (nodes.cur_node != NULL)) {
        found = (pid == nodes.cur_node->job_elem->pid);
        if(found) continue;
        nodes.prev_node = nodes.cur_node;
        nodes.cur_node  = nodes.cur_node->next;
        if(nodes.cur_node != NULL) nodes.next_node = nodes.cur_node->next;
    }
    if (nodes.cur_node == NULL) return NULL;
    else return nodes.cur_node->job_elem;
}

nil find_node(Plist list_inst, int pid){
    nil nodes;
    int found = 0;

    //if(DEBUG) printf("inside find node\n");

    if (list_inst->first_node == NULL){
        nodes.cur_node  = NULL;
        nodes.prev_node = NULL;
        nodes.next_node = NULL;
        return nodes;
    }else{
        //if(DEBUG) printf("inside else\n");
        nodes.cur_node  = list_inst->first_node;
        nodes.prev_node = NULL;
        nodes.next_node = nodes.cur_node->next;
        //if(DEBUG) printf("Done Init nodes\n"); 
        while(!found && (nodes.cur_node != NULL)) {
            //if(DEBUG) print_job(nodes.cur_node->job_elem);
            found = (pid == nodes.cur_node->job_elem->pid);
            if(found) continue;
            nodes.prev_node = nodes.cur_node;
            nodes.cur_node  = nodes.cur_node->next;
            if(nodes.cur_node != NULL) nodes.next_node = nodes.cur_node->next;
        }
        
        if (nodes.cur_node == NULL){ //in case no node suitable for this pid
           if (DEBUG) printf("didn't found suitable nodei\n");
           nodes.cur_node  = NULL;
           nodes.prev_node = NULL;
           nodes.next_node = NULL;
        }
        //else if(DEBUG) print_job(nodes.cur_node->job_elem);
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
    //if(DEBUG) printf("adding job to list\n"); 
    Pnode new_node = init_node();
    Pjob  new_job  = init_job(strlen(name));
    //positioning and linking
    new_node->next = NULL;
    new_node->job_elem = new_job;
    if (list_inst->num_of_jobs == 0){
        list_inst->last_node = new_node;
        list_inst->first_node = new_node;
    }
    else list_inst->last_node->next = new_node;
    list_inst->last_node = new_node; 
    list_inst->num_of_jobs = list_inst->num_of_jobs + 1; 
    //if(DEBUG)printf("done allocating memory and adding to list starting filling params\n");
    //if(DEBUG)printf("params: name: %s pid: %d suspended: %d\n", name, pid, suspended);
    //fill params 
    fill_job_params(new_job, name, pid, list_inst->num_of_jobs,suspended,0,UPDATE);
}

void align_idx(Plist list_inst){
    if (list_inst->first_node == NULL) return;           
    Pnode cur_node = list_inst->first_node;
    int i = 1;
    do {
        cur_node->job_elem->idx = i;
        cur_node = cur_node->next;
        i++;
    }
    while(cur_node != NULL); 
}

int remove_job(Plist list_inst, int pid){ //TODO: check if pid correct
    nil nil_res = find_node(list_inst, pid);
    //if(DEBUG) print_job(nil_res.cur_node->job_elem);
    if (nil_res.cur_node == NULL){
        if(DEBUG) printf("failed to find node\n");
        return FAILURE;
    }
   /***************repairing the list **********************/ 
    if(list_inst->num_of_jobs == 1 ){ //last job in list
        list_inst->first_node = NULL;
        list_inst->last_node  = NULL;
    }else{ //last or first job in list
        if(DEBUG) printf("going_to_remove: ");
        if(DEBUG) print_job(nil_res.cur_node->job_elem);
        if (nil_res.cur_node->job_elem->idx == 1)                                                              list_inst->first_node   = nil_res.next_node;
        if (nil_res.cur_node->job_elem->idx == list_inst->num_of_jobs)                                         list_inst->last_node    = nil_res.prev_node;
        if (nil_res.cur_node->job_elem->idx > 1 && nil_res.cur_node->job_elem->idx <= list_inst->num_of_jobs)  nil_res.prev_node->next = nil_res.next_node;
    }
    if(DEBUG) printf("going to remove: ");
    if(DEBUG) print_job(nil_res.cur_node->job_elem);
    /************releasing the memory**********************/
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
    printf("inside get last suspended for %d jobs\n",idx);
    while (!found && (idx>0)){ //TODO: think again of stop conditions
        cur_job = find_job_by_idx(list_inst, idx);
        printf("currently checking: ");
        if(DEBUG) print_job(cur_job);
        found = (SUSPENDED == cur_job->suspended);
        idx--;
    }
    if(!found) return NULL;
    printf("found: ");
     if(DEBUG) print_job(cur_job);

    return cur_job;
}

Pjob get_last_job (Plist list_inst){
    return list_inst->last_node->job_elem;
}

void print_job (Pjob job_inst){
    if (job_inst == NULL) {
        printf("nothing to print\n");
        return;
    }
    if (SUSPENDED == job_inst->suspended) printf ("[%d] %s %d %ld secs (Stopped)\n",job_inst->idx, job_inst->job_name, job_inst->pid, (time(NULL) - job_inst->start_time));
    else printf ("[%d] %s %d %ld secs\n",job_inst->idx, job_inst->job_name, job_inst->pid, (time(NULL) - job_inst->start_time));                                          
}

void print_jobs(Plist list_inst){
    int idx = 1;
    Pjob cur_job;
    //if(DEBUG) printf("inside print jobs, we have %d jobs to print\n", list_inst->num_of_jobs);
    if (list_inst->num_of_jobs == 0) return;
    while (idx <= list_inst->num_of_jobs){
        cur_job = find_job_by_idx(list_inst, idx);
        print_job(cur_job);
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

void suspend_job_in_list (Plist list_inst, int pid, int act_sus){
    //if(DEBUG) printf("inside_suspended\n");
    Pjob job_inst = find_job_by_pid(list_inst, pid);
    if(job_inst != NULL)job_inst->suspended = act_sus;
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
       printf("inside get_fg_job_pid: \n");
       printf("num of elems in list: %d \n", list_inst->num_of_jobs);
       if (list_inst->first_node == NULL) return 0;
       nil nodes; 
       int found = 0;
       nodes.cur_node = list_inst->first_node;
       nodes.prev_node = NULL;
       nodes.next_node = nodes.cur_node->next;
       while(!found && (nodes.cur_node != NULL)) {
           print_job(nodes.cur_node->job_elem);
           found = (FG == nodes.cur_node->job_elem->bg_fg);
           printf("found: %d\n", found);
           if(found) continue;
           nodes.prev_node = nodes.cur_node;
           nodes.cur_node  = nodes.cur_node->next;
           if(nodes.cur_node != NULL) nodes.next_node = nodes.cur_node->next;
           
       }
       if (nodes.cur_node == NULL) return 0;
       printf("The returned job is: \n");
       print_job(nodes.cur_node->job_elem);
       return nodes.cur_node->job_elem->pid;
   }

