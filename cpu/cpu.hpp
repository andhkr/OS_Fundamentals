#ifndef CPU_HPP
#define CPU_HPP

#include <ctime>
#include "kernel/proc.hpp"
#include "kernel/macros.hpp"
#include "kernel/lock.hpp"

struct cpu{
    int cpuid;    // hartid
    int ra;
    int sp;
    int pc;
    int s0,s1,s2,s3,s4,s5,s6,s7,s8,s9,s10,s11;
    //there are also many registers saved during trap handling
    // mentioned here only for context switch
    process* running_process=nullptr;
    volatile bool halt=false;          // stop cpu execution
    volatile bool ishalted = false;   // confirmation
    teplock* cpulock;
    //constructor
    cpu(){
        cpulock = new ticket_lock();
    }
    cpu(int);
    void finished();
    void in_cpu(process* p);
    void scheduled_proc();
};

extern cpu harts[cores];

#endif