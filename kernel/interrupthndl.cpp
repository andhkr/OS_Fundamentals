#include "interrupthndl.hpp"
#include "io.hpp"

void swtcontext(process*,process*);
extern ioreqrec iodevice;
volatile int haveinterrupt=0;       // interrupts bit per core
extern interrupt_info per_hart_infos[cores];
// array of function pointer for differet type of interrupt handling
// indexed by interrupt cause number.
void (*handler[6])(interrupt_info*) = {
    proc_fnshd_hndlr,
    io_request_hndlr,
    invalid_instr_hndlr,
    new_process_hndlr,
    sched_intr_hndlr,
    io_finished_hndlr
};

extern RR rnd_rbn;
// varoius interrupt handlers    
void proc_fnshd_hndlr(interrupt_info* info){
    int hart = info->hart;
    process* runn_p =  harts[hart].running_process;
    if(rnd_rbn.ready.get_count()!=0){
        process* new_proc = rnd_rbn.ready.dequeue();
        swtcontext(runn_p, new_proc);
        rnd_rbn.cpu_in(hart,new_proc);
    }else{
        harts[hart].running_process = nullptr;
        per_hart_infos[hart].p      = nullptr;
    }
}

void io_request_hndlr(interrupt_info* info){
    rnd_rbn.block(info->p);
    iodevice.rec(info);
    unsetbit(info->hart,haveinterrupt);
}

void invalid_instr_hndlr(interrupt_info* info){
    // yet to implement
}

void new_process_hndlr(interrupt_info* info){
    rnd_rbn.proc_in(info->p);
}

void sched_intr_hndlr(interrupt_info* info){    
    rnd_rbn.schdintr_handler();
}

void io_finished_hndlr(interrupt_info* info){
    // the corresponding process will be made to be in ready queue
    std::cout<<"io request of process "<<info->p->pid<<" is completed"<<std::endl;
    rnd_rbn.blocked.remove(info->p);
    std::cout<<"io request of process "<<info->p->pid<<" is completed"<<std::endl;
    rnd_rbn.Ready(info->p);
    std::cout<<"io request of process "<<info->p->pid<<" is completed"<<std::endl;
}

teplock* apnalocks[cores+1];

void apnalocks_initialisation(){
    for(int i = 0;i<cores+1;++i){
        apnalocks[i] = new ticket_lock();
    } 
}  

void free_apnalocks(){
    for (int i = 0;i<cores+1;++i){
        delete apnalocks[i];
    }
}

//interrupt handler: point where every interrupt come and scattered to various
//device to service interrupt
void* interrupt_handler(void* info){
    interrupt_info* tinfo = (interrupt_info*)info;;
    // std::cout<<"1."<<tinfo->hart<<" 2."<<tinfo->cause<<std::endl;
    apnalocks[tinfo->hart]->lock(nullptr);
    // std::cout<<tinfo->cause<<std::endl;
    handler[tinfo->cause](tinfo);
    // std::cout<<tinfo->cause<<std::endl;
    apnalocks[tinfo->hart]->unlock(nullptr);
    
    delete tinfo;
    info = nullptr;
    return nullptr;
}