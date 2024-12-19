#include "cpu.hpp"
#include "kernel/interrupthndl.hpp"

cpu harts[cores];

void cpu::finished(){
    running_process->finished = true;
}

cpu::cpu(int hart){
    cpuid = hart;
}

void cpu::in_cpu(process* p){
    std::cout<<"process "<<p->pid<<" scheduled"<<std::endl;
    running_process = p;
    task* curr_task = running_process->curr_task;
    while(curr_task){
        if(curr_task->type == 'I'){
            std::cout<<"process "<<p->pid<<" has made i/o request"<<std::endl;
            interrupt_info* info = new interrupt_info();
            info->cause  = i_o_request;
            info->hart   = cpuid;
            info->iotime = curr_task->time_req;
            interrupt_handler(info);
        }else{
            int time = curr_task->time_req;
            volatile long counter = 0;
            const clock_t start = clock();
            while(((clock()-start)*1000/CLOCKS_PER_SEC)<time)++counter; // job performing
        }
        curr_task = curr_task->next_task;
    }
    interrupt_info* info = new interrupt_info();
    info->cause  = proc_finished;
    info->hart   = cpuid;
    interrupt_handler(info);
    std::cout<<"process "<<p->pid<<" finished"<<std::endl;    
}

