#include "interrupthndl.hpp"
#include "roundrobin.hpp"
#include <iostream>
#include <pthread.h>
#include <sched.h>
#include <unistd.h>
#define number_of_procs 99

#define for_sched 0
#define for_interrupt 1

interrupt_info* infos[2];
pthread_cond_t conds[2];
pthread_mutex_t mutexes[2];

pthread_mutex_t per_hart_lock[cores];
extern pthread_cond_t per_hart_cond[cores];
extern interrupt_info per_hart_infos[cores];

void* mycpu(void* cpuid){
    int id = *(int*)cpuid;
    for(;;){
        pthread_mutex_lock(&per_hart_lock[id]);
        pthread_cond_wait(&per_hart_cond[id],&per_hart_lock[id]);
        harts[id].in_cpu(per_hart_infos[id].p);
        pthread_mutex_unlock(&per_hart_lock[id]);
    }
}


// void* scheduler_simulator(void* args){
    
// }

/*
in proc.hpp max number of process can be simulated is definded as macro
*/
RR rnd_rbn = RR();
int main(){
    apnalocks_initialisation();
    // pthread_t thrds[2];
    // int r_val1 = pthread_create(&thrds[1],NULL,cpu_simulator,NULL);
    // assert(r_val1 == 0);
    for(int i = 0;i<cores;++i){
        harts[i]=cpu(i);
    }

    // mutexes and condition variabel and infos initialisation;
    for(int i = 0;i<cores;++i){
        pthread_mutex_init(&per_hart_lock[i],NULL);
        pthread_cond_init(&per_hart_cond[i],NULL);
        per_hart_infos[i].cause = 0;
        per_hart_infos[i].hart = 0;
        per_hart_infos[i].p = nullptr;
    }

    pthread_t cpus[cores];
    int cpuid[cores];
    for(int i = 0;i<cores;++i){
        cpuid[i] = i;
        if(pthread_create(&cpus[i],NULL,mycpu,cpuid+i)){
            fprintf(stderr,"ERROR: thread allocation failed for cpu\n");
            exit(1);
        }

        cpu_set_t cpu_set;
        CPU_ZERO(&cpu_set);
        CPU_SET(i+1,&cpu_set);

        int result = pthread_setaffinity_np(cpus[i],sizeof(cpu_set_t),&cpu_set);
        assert(result == 0);
    }
    sleep(1);

    // rnd_rbn = RR();
    std::cout<<"Radha1"<<std::endl;
    
    std::cout<<"Radha2"<<std::endl;
    
    

    int slice = 50;
    

    


    // std::cout<<"Radha1"<<std::endl;
    
    // std::cout<<"Radha2"<<std::endl;
    
    proc_smltr processes = proc_smltr(number_of_procs);
    processes.sort_procs();
    
    // first process
    rnd_rbn.proc_in(&processes.process_list[0]);
    // std::cout<<"Radha5"<<std::endl;  
    // std::cout<<"Radha6"<<std::endl;  


    for(int i = 1;i<processes.n;++i){      
        // std::cout<<"Radha4"<<std::endl;  
        volatile long counter=0;
        int time_gap = processes.process_list[i].arrival-processes.process_list[i-1].arrival;
        interrupt_info* info = new interrupt_info();
        info->cause = new_process_in;
        info->hart  = 4;
        info->p     = &processes.process_list[i];

        // waiting for next process;
        clock_t start = clock();
        while(((clock()-start)*1000/CLOCKS_PER_SEC)!=time_gap)counter++; 
        // std::cout<<"Radha55"<<std::endl;
        interrupt_handler(info);
       
    }
    pthread_t thrds[2];
    int r_val2 = pthread_create(&thrds[0],NULL,scheduler,&slice);
    assert(r_val2==0);
    // pthread_join(thrds[0],NULL);
    for(int i = 0;i<cores;++i){
        pthread_join(cpus[i],NULL);
    }
    // pthread_join(thrds[1],NULL);
    free_apnalocks();
}