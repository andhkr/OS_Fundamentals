#include<iostream>
#include "queue/proc_queue.cpp"
#include <cassert>
#include "defs.hpp"


#define timer_interrupt 10

struct RR{
    proc_queue ready;
    proc_queue blocked;
    int time_slice = timer_interrupt;
    //default constructor
    RR(){}

    RR(int slice){
        assert(slice%timer_interrupt == 0);
        time_slice = slice;

    }  

    void proc_in(process* p){
        ready.enqueue(p);
    }

    void schdintr_handler(){
        for(int i = 0;i<cores;++i){
            process* running = harts[i].running_process;
            ready.enqueue(running);
            process* ready_proc = ready.dequeue();
            running->swtcontext(ready_proc);
            harts[i].in_cpu(ready_proc);
        }
    }

    void block(process* p){
        process* ready_proc = ready.dequeue();
        p->swtcontext(ready_proc);
        harts[hart].in_cpu(ready_proc);
        blocked.enqueue(p);
    }

    void ready(process* p){
        ready.enqueue(p);
    }
};

RR rnd_rbn = RR();

void scheduler(int slice){    
    for(;;){
        volatile long counter = 0;
        const clock_t start = clock();
        while(((clock()-start)*1000/CLOCKS_PER_SEC)<rnd_rbn.time_slice) counter++;
        // scheduler interrupt

    }
}
