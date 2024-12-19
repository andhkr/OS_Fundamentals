#include "interrupthndl.hpp"
#include "roundrobin.hpp"
#include <iostream>
#include <pthread.h>
#include <sched.h>
#include <unistd.h>
#include <signal.h>

#define number_of_procs 99

#define for_sched 0
#define for_interrupt 1

pthread_mutex_t per_hart_lock[cores];
extern pthread_cond_t per_hart_cond[cores];
extern interrupt_info per_hart_infos[cores];

void* mycpu(void* cpuid){
    int id = *(int*)cpuid;
    for(;;){
        pthread_mutex_lock(&per_hart_lock[id]);

        pthread_cond_wait(&per_hart_cond[id],&per_hart_lock[id]);

        //start to run process
        harts[id].in_cpu(per_hart_infos[id].p);

        pthread_mutex_unlock(&per_hart_lock[id]);
    }
}

/*
in proc.hpp max number of process can be simulated is definded as macro
*/
RR rnd_rbn = RR();
int main(){
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

    //now simulating process
    proc_smltr processes = proc_smltr(number_of_procs);
    // sorting process according to their arrival time
    processes.sort_procs();
    
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

    // signal(SIGINT,)
    for(int i = 0;i<cores;++i){
        pthread_join(cpus[i],NULL);
    }
    free_apnalocks();
}