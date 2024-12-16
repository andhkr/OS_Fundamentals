#ifndef INTERRUPT_HNDL_HPP
#define INTERRUPT_HNDL_HPP

#include "allheaders.hpp"

//interrupt cause number
#define proc_finished 0
#define i_o_request  1
#define invalid_instr 2
#define new_process_in  3
#define sched_intr  4
#define i_o_finished 5

#include "cpu/cpu.hpp"

extern RR rnd_rbn;
extern process* new_process;

// struct used by every device for interrupt generation
struct interrupt_info{
    int cause;
    int hart;
    process* p = nullptr;
};

//various interrupt handlers    
void proc_fnshd_hndlr(interrupt_info* info);

void io_request_hndlr(interrupt_info* info);

void invalid_instr_hndlr(interrupt_info* info);

void new_process_hndlr(interrupt_info* info);

void sched_intr_hndlr(interrupt_info* info);

void io_finished_hndlr(interrupt_info* info);

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

/*
make the lock for each interrupt handler function
for efficiency
*/
//locks for every interrupt handler:
const int no_of_hndlrs = NELIA(handler);

teplock* apnalocks[no_of_hndlrs];

void apnalocks_initialisation(); 

void free_apnalocks();



//interrupt handler: point where every interrupt come and scattered to various
//device to service interrupt
void* interrupt_handler(void* info);

#endif