#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>

// #include "/home/xx/Desktop/xx/task_lab1/task_lab1/labs_headers/message.h"
// #include "/home/xx/Desktop/xx/task_lab1/task_lab1/labs_headers/log.h"
// #include "/home/xx/Desktop/xx/task_lab1/task_lab1/labs_headers/process.h"
#include "message.h"
#include "log.h"
#include "process.h"


void parent_work(int count_nodes)
{

    Message msg;
    for(int i=1;i<count_nodes;i++){
        while(true){
            receive(i,&msg);
            if(msg.s_header.s_magic == MESSAGE_MAGIC && msg.s_header.s_type == STARTED){
                
                break;
            }else{
                continue;
            }
        }
        
    }
    for(int i=1;i<count_nodes;i++){
        while(true){
            receive(i,&msg);
            if(msg.s_header.s_magic == MESSAGE_MAGIC && msg.s_header.s_type == DONE){
                
                break;
            }else{
                continue;
            }
        }
    }

}

void child_work(struct child_arguments args)
{
    local_id self_id = args.self_id;
    int count_nodes = args.count_nodes;
    pid_t self_pid = getpid();
    pid_t parent_pid = getppid();
    char buf[BUF_SIZE];
    Message msg;
    // Phase 1 
    snprintf(buf,BUF_SIZE,log_started_fmt,0,self_id,self_pid,parent_pid,0);
    fill_message(&msg,STARTED,0,buf,strlen(buf));
    send_multicast(&msg);
    shared_logger(buf);

    for(int i=1;i<count_nodes;i++){
        if(i==self_id) continue;
        while(true){
            receive(i,&msg);
            if(msg.s_header.s_magic == MESSAGE_MAGIC && msg.s_header.s_type == STARTED){
                break;
            }else{
                continue;
            }
        }

        
    }
    printf(log_received_all_started_fmt,0,self_id);

    // Phase 2

    // Phase 3
    snprintf(buf,BUF_SIZE,log_done_fmt,0,self_id,0);
    fill_message(&msg,DONE,0,buf,strlen(buf));
    send_multicast(&msg);
    shared_logger(buf);
    for(int i=1;i<count_nodes;i++){
        if(i==self_id) continue;
        while(true){
            receive(i,&msg);
            if(msg.s_header.s_magic == MESSAGE_MAGIC && msg.s_header.s_type == DONE){

                break;
            }else{
                continue;
            }
        }
        
    }
    printf(log_received_all_done_fmt,0,self_id);

}

/*
export LD_LIBRARY_PATH="$LD_LIBRARY_PATH:./libdistributedmodel"
LD_PRELOAD=./libdistributedmodel.so ./lab -l 1 -p 3
*/
