#ifndef CPU_HPP
#define CPU_HPP

#include <ctime>
#include "kernel/proc.hpp"
#include "kernel/macros.hpp"

struct cpu{
    int cpuid;    // hartid
    int ra;
    int sp;
    int pc;
    int s0,s1,s2,s3,s4,s5,s6,s7,s8,s9,s10,s11;
    //there are also many registers saved during trap handling
    // mentioned here only for context switch
    process* running_process=nullptr;

    //constructor
    cpu(){}
    cpu(int);
    void finished();
    void in_cpu(process* p);
};

extern cpu harts[cores];

#endif