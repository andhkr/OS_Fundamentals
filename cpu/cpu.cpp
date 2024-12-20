#include "cpu.hpp"
#include "kernel/interrupthndl.hpp"

extern interrupt_info per_hart_infos[cores];
cpu harts[cores];

void cpu::finished(){
    running_process->finished = true;
}

cpu::cpu(int hart){
    cpuid = hart;
}

void cpu::in_cpu(process* p){
    std::cout<<"process "<<p->pid<<" scheduled on cpu"<<cpuid<<std::endl;

    running_process = p;
    p->hart         = cpuid;
    task* curr_task = running_process->curr_task;
    while(curr_task){
        // std::cout<<"cpuid1"<<std::endl;
        // char type = curr_task->type;
        // std::cout<<type<<"1"<<std::endl;
        if(curr_task->type == 'I'){
            std::cout<<"process "<<p->pid<<" has made i/o request"<<std::endl;
            interrupt_info* info = new interrupt_info();
            info->cause  = i_o_request;
            info->hart   = cpuid;
            info->p      = p;
            info->iotime = curr_task->time_req;
            interrupt_handler(info);
            return;       // cpu is stopping this process to run
        }else{
            // std::cout<<type<<"2"<<std::endl;
            int time = curr_task->time_req;
            // std::cout<<type<<"22"<<std::endl;
            long counter = 0;
            clock_t start = clock();
            while(((clock()-start)*1000/CLOCKS_PER_SEC)<time)++counter; // job performing
        }
        // std::cout<<type<<"3"<<std::endl;
        curr_task = curr_task->next_task;
        
    }

    interrupt_info* info = new interrupt_info();
    info->cause  = proc_finished;
    info->hart   = cpuid;
    interrupt_handler(info);

    std::cout<<"process "<<p->pid<<" finished"<<std::endl;    
}

