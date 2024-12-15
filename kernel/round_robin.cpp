#include "round_robin.hpp"

RR::RR(int slice){
    assert(slice%timer_interrupt == 0);
    time_slice = slice;

}  

void RR::proc_in(process* p){
    ready.enqueue(p);
}

void RR::schdintr_handler(){
    for(int i = 0;i<cores;++i){
        process* running = harts[i].running_process;
        ready.enqueue(running);
        process* ready_proc = ready.dequeue();
        running->swtcontext(ready_proc);
        ready_proc->hart = i;
        harts[i].in_cpu(ready_proc);
    }
}

void RR::block(process* p){
    if(ready.get_count()!=0){
        process* ready_proc = ready.dequeue();
        p->swtcontext(ready_proc);
        harts[p->hart].in_cpu(ready_proc);
        ready_proc->hart = p->hart;
        blocked.enqueue(p);
    }else{
        //update context
        p->savecontext();
        blocked.enqueue(p);
    }
}

void RR::Ready(process* p){
    ready.enqueue(p);
}



void scheduler(int slice){    
    for(;;){
        volatile long counter = 0;
        const clock_t start = clock();
        while(((clock()-start)*1000/CLOCKS_PER_SEC)<rnd_rbn.time_slice) counter++;
        // scheduler interrupt

    }
}
