#include "interrupthndl.hpp"
#include <iostream>
#define number_of_procs 4

/*
in proc.hpp max number of process can be simulated is definded as macro
*/
int main(){
    std::cout<<"Radha1"<<std::endl;
    apnalocks_initialisation();
    std::cout<<"Radha2"<<std::endl;
    for(int i = 0;i<cores;++i){
        harts[i]=cpu(i);
    }
    proc_smltr processes = proc_smltr(number_of_procs);
    processes.sort_procs();
    
    // first process
    rnd_rbn.proc_in(&processes.process_list[0]);
    std::cout<<"Radha5"<<std::endl;  
    scheduler(3*timer_interrupt);
    std::cout<<"Radha6"<<std::endl;  
    for(int i = 1;i<processes.n;++i){      
        std::cout<<"Radha4"<<std::endl;  
        volatile long counter=0;
        int time_gap = processes.process_list[i].arrival-processes.process_list[i-1].arrival;
        interrupt_info* info = new interrupt_info();
        info->cause = new_process_in;
        info->hart  = 4;
        info->p     = &processes.process_list[i];

        // waiting for next process;
        clock_t start = clock();
        while(((clock()-start)*1000/CLOCKS_PER_SEC)!=time_gap)counter++; 
        interrupt_handler(info);
        std::cout<<"Radha5"<<std::endl;
    }
    free_apnalocks();
}