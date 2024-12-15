#include <string>
#include <iostream>
#include "proc.hpp"
#include <cassert>

//task implimentation:
//parameterised constructor
task::task(char a_type, int a_time,task* a_next_task)
:type(a_type),time_req(a_time),next_task(a_next_task)
{}



//process implimentation:
//parameterised constructor
process::process(int a_pid,int a_arrival,task a_head)
:pid(a_pid),arrival(a_arrival),tasklisthead(a_head)
{}

// save the context of process and restore the other
void process::swtcontext(process* new_proc){

    registers->pc=harts[hart].pc;
    registers->ra=harts[hart].ra;
    registers->s0=harts[hart].s0;
    registers->s1=harts[hart].s1;
    registers->s2=harts[hart].s2;
    registers->s3=harts[hart].s3;
    registers->s4=harts[hart].s4;
    registers->s5=harts[hart].s5;
    registers->s6=harts[hart].s6;
    registers->s7=harts[hart].s7;
    registers->s8=harts[hart].s8;
    registers->s9=harts[hart].s9;
    registers->s10=harts[hart].s10;
    registers->s11=harts[hart].s11;

    harts[hart].ra = new_proc->registers->ra;
    harts[hart].s0 = new_proc->registers->s0;
    harts[hart].s1 = new_proc->registers->s1;
    harts[hart].s2 = new_proc->registers->s2;
    harts[hart].s3 = new_proc->registers->s3;
    harts[hart].s4 = new_proc->registers->s4;
    harts[hart].s5 = new_proc->registers->s5;
    harts[hart].s6 = new_proc->registers->s6;
    harts[hart].s7 = new_proc->registers->s7;
    harts[hart].s8 = new_proc->registers->s8;
    harts[hart].s9 = new_proc->registers->s9;
    harts[hart].s10= new_proc->registers->s10;
    harts[hart].s11= new_proc->registers->s11;
}
// update context when singel process for context switch handel
void process::savecontext(){
    registers->pc=harts[hart].pc;
    registers->ra=harts[hart].ra;
    registers->s0=harts[hart].s0;
    registers->s1=harts[hart].s1;
    registers->s2=harts[hart].s2;
    registers->s3=harts[hart].s3;
    registers->s4=harts[hart].s4;
    registers->s5=harts[hart].s5;
    registers->s6=harts[hart].s6;
    registers->s7=harts[hart].s7;
    registers->s8=harts[hart].s8;
    registers->s9=harts[hart].s9;
    registers->s10=harts[hart].s10;
    registers->s11=harts[hart].s11;
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

        for(int j = 1;j<number_of_task;++j){
            int i_type = rand()%2;
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
    }
    delete[] process_list;
}

// print process
void proc_smltr::print(){
    for(int i = 0;i<n;++i){
        process_list[i].print_process();
    }
}


int main(){
    proc_smltr processes = proc_smltr(4);
    processes.print();
}