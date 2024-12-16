#include "allheaders.hpp"
#include "interrupt_hndl.hpp"
#include "cpu/cpu.hpp"
#define number_of_procs 4

/*
in proc.hpp max number of process can be simulated is definded as macro
*/
int main(){
    for(int i = 0;i<cores;++i){
        harts[i]=cpu(i);
    }
    proc_smltr processes = proc_smltr(number_of_procs);
    processes.sort_procs();
    for(int i = 0;i<processes.n;++i){
        rnd_rbn.proc_in(&processes.process_list[i]);
        clock_t start = clock();
        if(i < processes.n){
            volatile long counter=0;
            int time_gap = processes.process_list[i+1].arrival-processes.process_list[i].arrival;
            while(((clock()-start)*1000/CLOCKS_PER_SEC)!=time_gap)counter++; 
        }
    }
    free_apnalocks();
}