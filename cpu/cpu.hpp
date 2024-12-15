#ifndef CPU_HPP
#define CPU_HPP

#include <ctime>
#include "kernel/proc.hpp"
#define cores 4

struct cpu{
    int cpuid;
    int ra;
    int sp;
    int pc;
    int s0,s1,s2,s3,s4,s5,s6,s7,s8,s9,s10,s11;
    //there are also many registers saved during trap handling
    // mentioned here only for context switch
    process* running_process;
    cpu(){}
    void finished(){
        running_process->finished = true;
    }

    void in_cpu(process* p){
        running_process = p;
        task* curr_task = running_process->curr_task;
        while(curr_task){
            if(curr_task->type == 'I'){
                // i/o interrupt;
            }else{
                int time = curr_task->time_req;
                volatile long counter = 0;
                const clock_t start = clock();
                while(((clock()-start)*1000/CLOCKS_PER_SEC)<time)++counter; // job performing
            }
            curr_task = curr_task->next_task;
        }
        // finished interrupt;
    }
};

cpu harts[cores];

int hart = 0;
#endif