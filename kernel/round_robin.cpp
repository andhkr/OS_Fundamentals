#include "roundrobin.hpp"
#include "interrupthndl.hpp"
#include <pthread.h>
#include <unistd.h>

interrupt_info per_hart_infos[cores];
pthread_cond_t per_hart_cond[cores];

extern std::mutex print_mutex;
extern std::atomic<bool> run_threads;

// save the context of process and restore the other
void swtcontext(process* old, process* new_proc){
    old->registers->pc=harts[old->hart].pc;
    old->registers->ra=harts[old->hart].ra;
    old->registers->s0=harts[old->hart].s0;
    old->registers->s1=harts[old->hart].s1;
    old->registers->s2=harts[old->hart].s2;
    old->registers->s3=harts[old->hart].s3;
    old->registers->s4=harts[old->hart].s4;
    old->registers->s5=harts[old->hart].s5;
    old->registers->s6=harts[old->hart].s6;
    old->registers->s7=harts[old->hart].s7;
    old->registers->s8=harts[old->hart].s8;
    old->registers->s9=harts[old->hart].s9;
    old->registers->s10=harts[old->hart].s10;
    old->registers->s11=harts[old->hart].s11;

    harts[old->hart].ra = new_proc->registers->ra;
    harts[old->hart].s0 = new_proc->registers->s0;
    harts[old->hart].s1 = new_proc->registers->s1;
    harts[old->hart].s2 = new_proc->registers->s2;
    harts[old->hart].s3 = new_proc->registers->s3;
    harts[old->hart].s4 = new_proc->registers->s4;
    harts[old->hart].s5 = new_proc->registers->s5;
    harts[old->hart].s6 = new_proc->registers->s6;
    harts[old->hart].s7 = new_proc->registers->s7;
    harts[old->hart].s8 = new_proc->registers->s8;
    harts[old->hart].s9 = new_proc->registers->s9;
    harts[old->hart].s10= new_proc->registers->s10;
    harts[old->hart].s11= new_proc->registers->s11;
}
// update context when singel process for context switch handel
void savecontext(process* old){
    old->registers->pc=harts[old->hart].pc;
    old->registers->ra=harts[old->hart].ra;
    old->registers->s0=harts[old->hart].s0;
    old->registers->s1=harts[old->hart].s1;
    old->registers->s2=harts[old->hart].s2;
    old->registers->s3=harts[old->hart].s3;
    old->registers->s4=harts[old->hart].s4;
    old->registers->s5=harts[old->hart].s5;
    old->registers->s6=harts[old->hart].s6;
    old->registers->s7=harts[old->hart].s7;
    old->registers->s8=harts[old->hart].s8;
    old->registers->s9=harts[old->hart].s9;
    old->registers->s10=harts[old->hart].s10;
    old->registers->s11=harts[old->hart].s11;
}

RR::RR(int slice){
    assert(slice%timer_interrupt == 0);
    time_slice = slice;

}  

void RR::proc_in(process* p){
    for(int i = 0;i<cores;++i){
        if(harts[i].running_process == nullptr){
            cpu_in(i,p);
            return;
        }
    }
    ready.enqueue(p);
}

void RR::schdintr_handler(){
    for(int i = 0;i<cores;++i){
        process* running = harts[i].running_process;
        if(running != nullptr){
            ready.enqueue(running);
            process* ready_proc = ready.dequeue();
            swtcontext(running, ready_proc);
            cpu_in(i,ready_proc);
        }
    }
}

void RR::block(process* p){
    if(ready.get_count()!=0){
        process* ready_proc = ready.dequeue();
        swtcontext(p,ready_proc);
        cpu_in(p->hart,ready_proc);
        blocked.insert(p);
    }else{
        //update context
        savecontext(p);
        blocked.insert(p);
    }
}

void RR::Ready(process* p){
    ready.enqueue(p);
}

void RR::cpu_in(int hart,process* p){
    per_hart_infos[hart].p = p;
    p->hart = hart;
    pthread_cond_signal(&per_hart_cond[hart]);
}

extern RR rnd_rbn;

void* scheduler(void* args){    
    rnd_rbn.time_slice = *(int*)args;
    int rslice = rnd_rbn.time_slice;
    while(run_threads){
        volatile long counter = 0;
        clock_t start = clock();
        interrupt_info* info = new interrupt_info();
        info->cause  = sched_intr;
        info->hart   = 4;
        while(((clock()-start)*1000/CLOCKS_PER_SEC)<rslice) counter++;        
        interrupt_handler(info);
    }

    {
        std::lock_guard<std::mutex> lock(print_mutex);
        std::cout<<"Now Scheduler is off"<<std::endl;
    }
    return nullptr;
}
