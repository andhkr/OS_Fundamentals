#include "cpu.hpp"
#include "kernel/interrupthndl.hpp"
volatile int coresinuse = 0;
extern queue<process*> per_hart_infos[cores];
extern RR rnd_rbn;
cpu harts[cores];

void cpu::finished(){
    running_process->finished = true;
}

cpu::cpu(int hart){
    cpuid = hart;
    cpulock = new ticket_lock();
}

void cpu::in_cpu(process* p){
    cpulock->lock(nullptr); 
    if(running_process){
        rnd_rbn.Ready(p);
        cpulock->unlock(nullptr);
        return;
    }
    running_process = p;
    cpulock->unlock(nullptr);

    std::cout<<"process "<<p->pid<<" scheduled on cpu"<<cpuid<<std::endl;
    
    task* curr_task = running_process->curr_task;
    while(curr_task){

        if(halt){
            ishalted = true;// stop signal, by scheduler
            break;
        }

        if(curr_task->type == 'I'){

            if(halt){
                ishalted = true;
                break;    
            }

            setbit(cpuid,haveinterrupt);
            std::cout<<"process "<<p->pid<<" has made i/o request"<<std::endl;
            interrupt_info* info = new interrupt_info();
            info->cause  = i_o_request;
            info->hart   = cpuid;
            info->p      = p;
            info->iotime = curr_task->time_req;
            running_process->curr_task = curr_task->next_task;
            interrupt_handler(info);
            return;       // cpu is stopping this process to run
        }else{

            // std::cout<<type<<"2"<<std::endl;
            int time = curr_task->time_req;
            // std::cout<<type<<"22"<<std::endl;
            long counter = 0;
            clock_t start = clock();
            while(((clock()-start)*1000/CLOCKS_PER_SEC)<time){
                if(halt){
                    ishalted = true;
                    break;        // stop signal by scheduler
                }       
                ++counter; // job performing
            }
        }
        // std::cout<<type<<"3"<<std::endl;
        curr_task = curr_task->next_task;
        // std::cout<<" anup bhau "<<running_process<<std::endl;
        running_process->curr_task = curr_task;
        if(halt){
            ishalted = true;
            break;       // stop signal by scheduler
        }
    }

    if(!halt){
        interrupt_info* info = new interrupt_info();
        info->cause  = proc_finished;
        info->hart   = cpuid;
        interrupt_handler(info);    
    }
}

