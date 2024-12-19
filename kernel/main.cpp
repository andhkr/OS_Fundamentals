#include "interrupthndl.hpp"
#include "roundrobin.hpp"
#include <iostream>
#include <pthread.h>
#include <sched.h>
#include <unistd.h>
#include <csignal>
#include <atomic>
#include <mutex>

#define number_of_procs 4

std::mutex print_mutex;                      // since std::cout is not thread safe
std::atomic<bool> run_threads(true);         // atomic variables so that read , write are thread safe

pthread_mutex_t per_hart_lock[cores];
extern pthread_cond_t per_hart_cond[cores];
extern interrupt_info per_hart_infos[cores];

void* mycpu(void* cpuid){
    int id = *(int*)cpuid;
    while(run_threads){
        pthread_mutex_lock(&per_hart_lock[id]);

        pthread_cond_wait(&per_hart_cond[id],&per_hart_lock[id]);

        //start to run process
        // if info have process then schedule
        if(per_hart_infos[id].p)
        harts[id].in_cpu(per_hart_infos[id].p);

        pthread_mutex_unlock(&per_hart_lock[id]);
    }

    /*
    if in between process power off signal comes so there may be some thread waiting 
    befor mutex and signal by signal handler may get waste so in that case each 
    exiting thread will broadcaste
    */
    for(int i = 0;i<cores;++i){
        pthread_cond_signal(&per_hart_cond[i]);
    }

    {
        /*
        at a time only one thread can use std::cout so that 
        output is consistent and lock_guard will release the lock
        automatically as it goes out of scope
        */
        std::lock_guard<std::mutex> lock(print_mutex);
        std::cout<<"CPU "<<id<<" is now off"<<std::endl;
    }
    return nullptr;
}

// power off
void signalhandler(int signum){
    run_threads = false;
    for(int i = 0;i<cores;++i){
        pthread_cond_signal(&per_hart_cond[i]);
    }
}
/*
in proc.hpp max number of process can be simulated is definded as macro
*/
RR rnd_rbn = RR();
int main(){
    // how to power off
    struct sigaction sa;
    sa.sa_handler = signalhandler;    
    sa.sa_flags   = 0;        // no other flags to configure behaviour of handler    
    sigemptyset(&sa.sa_mask); // no other signals will be masked while handling the signal

    sigaction(SIGINT,&sa,nullptr);   // --> for ctrl+c
    sigaction(SIGTSTP,&sa,nullptr);  // --> for ctrl+z

    apnalocks_initialisation();                       // locks initialisation

    for(int i = 0;i<cores;++i){                      // cpu initialisation
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

    // creating thread for each cpu
    pthread_t cpus[cores];
    int cpuid[cores];
    for(int i = 0;i<cores;++i){
        cpuid[i] = i;
        if(pthread_create(&cpus[i],NULL,mycpu,cpuid+i)){
            fprintf(stderr,"ERROR: thread allocation failed for cpu\n");
            exit(1);
        }

        // threads affinity set
        cpu_set_t cpu_set;       
        CPU_ZERO(&cpu_set);
        CPU_SET(i+1,&cpu_set);

        if(pthread_setaffinity_np(cpus[i],sizeof(cpu_set_t),&cpu_set)){
            fprintf(stderr,"ERROR: affinity set of threads for cpus failed\n");
            exit(1);
        }
        
    }
    sleep(1);

    // creating scheduler thread
    int slice = 5*timer_interrupt;
    pthread_t sched_thrd;
    if(pthread_create(&sched_thrd,NULL,scheduler,&slice)){
        fprintf(stderr,"ERROR:Scheduler thread creation Failed\n");
        exit(1);
    }

    // setting cpu affinity for scheduler
    cpu_set_t cpu_set;
    CPU_ZERO(&cpu_set);
    CPU_SET(cores+1,&cpu_set);

    if(pthread_setaffinity_np(sched_thrd,sizeof(cpu_set_t),&cpu_set)){
        fprintf(stderr,"ERROR:Scheduler affinity set fails\n");
        exit(1);
    }

    //now simulating process
    proc_smltr processes = proc_smltr(number_of_procs);
    // sorting process according to their arrival time
    processes.sort_procs();
    processes.print();
    // first process
    rnd_rbn.proc_in(&processes.process_list[0]);

    for(int i = 1;i<processes.n;++i){      

        //time gap between arrival of two process
        int time_gap = processes.process_list[i].arrival-processes.process_list[i-1].arrival;

        // now making info for interrupt for new process scheduling
        interrupt_info* info = new interrupt_info();
        info->cause = new_process_in;
        info->hart  = 4;                              // total four cores , the 5th core is assumed for os
        info->p     = &processes.process_list[i];

        // waiting for next process;
        long counter=0;
        clock_t start = clock();
        while(((clock()-start)*1000/CLOCKS_PER_SEC)!=time_gap)counter++; 

        interrupt_handler(info);       
    }

    //  joining all cpus
    for(int i = 0;i<cores;++i){
        pthread_join(cpus[i],NULL);
    }
    // joining scheduler
    pthread_join(sched_thrd,nullptr);
    free_apnalocks();
    std::cout<<"Power off"<<std::endl;
    return 0;
}