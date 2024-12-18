#include "interrupthndl.hpp"

void swtcontext(process*,process*);



// array of function pointer for differet type of interrupt handling
// indexed by interrupt cause number.
void (*handler[])(interrupt_info*) = {
    proc_fnshd_hndlr,
    io_request_hndlr,
    invalid_instr_hndlr,
    new_process_hndlr,
    sched_intr_hndlr,
    io_finished_hndlr
};

// varoius interrupt handlers    
void proc_fnshd_hndlr(interrupt_info* info){
    int hart = info->hart;
    process* runn_p =  harts[hart].running_process;
    if(rnd_rbn.ready.get_count()!=0){
        process* new_proc = rnd_rbn.ready.dequeue();
        swtcontext(runn_p, new_proc);
        harts[hart].running_process = new_proc;
    }else{
        harts[hart].running_process = nullptr;
    }
}

void io_request_hndlr(interrupt_info* info){
    int hart = info->hart;
    rnd_rbn.block(harts[hart].running_process);
    //call disk for service (yet to impliment)
}

void invalid_instr_hndlr(interrupt_info* info){
    // yet to implement
}

void new_process_hndlr(interrupt_info* info){

    rnd_rbn.ready.enqueue(info->p);
}

void sched_intr_hndlr(interrupt_info* info){
    rnd_rbn.schdintr_handler();
}

void io_finished_hndlr(interrupt_info* info){
    // the corresponding process will be made to be in ready queue
    rnd_rbn.blocked.remove(info->p);
    rnd_rbn.Ready(info->p);
}

teplock* apnalocks[cores+1];

void apnalocks_initialisation(){
    for(int i = 0;i<cores+1;++i){
        apnalocks[i] = new ticket_lock();
    } 
}  

void free_apnalocks(){
    for (int i = 0;i<cores;++i){
        delete apnalocks[i];
    }
}

//interrupt handler: point where every interrupt come and scattered to various
//device to service interrupt
void* interrupt_handler(void* info){
    interrupt_info* tinfo = (interrupt_info*)info;
    apnalocks[tinfo->hart]->lock(nullptr);
    handler[tinfo->cause](tinfo);
    apnalocks[tinfo->hart]->unlock(nullptr);
    delete tinfo;
    info = nullptr;
    return nullptr;
}