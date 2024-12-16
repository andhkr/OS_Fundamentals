#include "allheaders.hpp"

//interrupt cause number
#define proc_finished 0
#define i_o_request  1
#define invalid_instr 2
#define new_process_in  3
#define sched_intr  4
#define i_o_finished 5

#include "cpu/cpu.hpp"
/*
make the lock for each interrupt handler function
for efficiency
*/
extern RR rnd_rbn;
extern process* new_process;

    
void proc_fnshd_hndlr(int hart){
    process* runn_p =  harts[hart].running_process;
    if(rnd_rbn.ready.get_count()!=0){
        process* new_proc = rnd_rbn.ready.dequeue();
        runn_p->swtcontext(new_proc);
        harts[hart].running_process = new_proc;
    }else{
        harts[hart].running_process = nullptr;
    }
}

void io_request_hndlr(int hart){
    rnd_rbn.block(harts[hart].running_process);
    //call disk for service (yet to impliment)
}

void invalid_instr_hndlr(int hart){
    // yet to implement
}

void new_process_hndlr(int hart){
    rnd_rbn.ready.enqueue(new_process);
}

void sched_intr_hndlr(int hart){
    rnd_rbn.schdintr_handler();
}

void io_finished_hndlr(int hart){
    // the corresponding process will be made to be in ready queue
}

// array of function pointer for differet type of interrupt handling
// indexed by interrupt cause number.
void (*handler[])(int) = {
    proc_fnshd_hndlr,
    io_request_hndlr,
    invalid_instr_hndlr,
    new_process_hndlr,
    sched_intr_hndlr,
    io_finished_hndlr
};

// number of element in an array
#define NELIA(x)(sizeof(x)/sizeof(x[0]))    
ticket_lock lock = ticket_lock();
teplock& apnalock = lock;

//locks for every interrupt handler:
const int no_of_hndlrs = NELIA(handler);

teplock* apnalocks[no_of_hndlrs];

void apnalocks_initialisation(){
    for(int i = 0;i<no_of_hndlrs;++i){
        apnalocks[i] = new ticket_lock();
    } 
}  

void free_apnalocks(){
    delete[] apnalocks;
}

struct interrupt_info{
    int cause;
    int hart;
};

void* interrupt_handler(void* info){
    interrupt_info* tinfo = (interrupt_info*)info;
    apnalocks[tinfo->cause]->lock(nullptr);
    handler[tinfo->cause](tinfo->hart);
    apnalocks[tinfo->cause]->unlock(nullptr);
    delete tinfo;
}