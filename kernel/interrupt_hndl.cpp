#include "allheaders.hpp"

//interrupt cause number
#define proc_finished 0
#define i_o_request  1
#define invalid_instr 2
#define new_process  3
#define sched_intr  4
#define i_o_finished 5

#include "cpu/cpu.hpp"

extern RR rnd_rbn;
extern teplock& apnalock;

struct intrrupt_handler{
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
    }

    void invalid_instr_hndlr(int hart){
        // yet to implement
    }

};

intrrupt_handler intr_hndlr;

void interrupt_handler(int intr_cause){
    // intr_hndlr.arr[intr_cause]
}