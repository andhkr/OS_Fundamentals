#include <string>
#include <iostream>
#include "proc.hpp"
#include <cassert>
#include <algorithm>

//task implimentation:
//parameterised constructor
task::task(char a_type, int a_time,task* a_next_task)
:type(a_type),time_req(a_time),next_task(a_next_task)
{}

//process implimentation:
//parameterised constructor
process::process(int a_pid,int a_arrival,task a_head)
:pid(a_pid),arrival(a_arrival),tasklisthead(a_head)
{
    registers = new context();
}


// print
void process::print_process(){
    std::cout<<"P"<<pid<<" "<<arrival<<" ";
    task* curr = &tasklisthead;
    while(curr){
        std::cout<<curr->type<<" "<<curr->time_req<<" ";
        curr = curr->next_task;
    }
    std::cout<<std::endl;
}




//proc_smltr implimentation:
// constructor
proc_smltr::proc_smltr(int n){
    assert(n<=max_procs);
    this->n = n;
    process_list = new process[n];

    for(int i = 0;i<n;++i){
        process_list[i].pid = i;
        int arrival = rand()%(max_arrival_time_unit*(i+1));
        process_list[i].arrival = arrival;

        // tasklist
        int number_of_task = (1+rand())%max_task_per_process; 
        int job_lenght     = (1+rand())%max_time_per_task;
        process_list[i].tasklisthead = task('C',job_lenght,NULL);

        task* curr_proc = &process_list[i].tasklisthead;

        // curr_task
        process_list[i].curr_task = curr_proc;

        for(int j = 1;j<number_of_task;++j){
            char type = (rand() % 2 == 0) ? 'C' : 'I';

            job_lenght     = (1+rand())%max_time_per_task;
            curr_proc->next_task = new task(type,job_lenght,NULL);
            curr_proc = curr_proc->next_task;
        }
    }
}

// destructor
proc_smltr::~proc_smltr(){
    for (int i = 0;i<n;++i){
        task* curr = process_list[i].tasklisthead.next_task;
        while(curr){
            task* prev = curr;
            curr = curr->next_task;
            delete prev;
        }
        delete process_list[i].registers;
    }
    delete[] process_list;
}

// print process
void proc_smltr::print(){
    for(int i = 0;i<n;++i){
        process_list[i].print_process();
    }
}

// sort processes with respect to arrival time
void proc_smltr::sort_procs(){
    // take time to implement merge sort
    auto compare = [](process p1,process p2){
        return p1.arrival<p2.arrival;
    };
    
    std::sort(process_list,process_list+n,compare);
}

