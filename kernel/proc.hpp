#ifndef DEFS_HPP
#define DEFS_HPP

#include "cpu/cpu.hpp"
#include <cstdint>

// proc_simltr.cpp

#define max_procs             100 // maximum process system can handel
#define max_task_per_process  20
#define max_arrival_time_unit 100
#define max_time_per_task     1000

/*
P1 5 C 20 I 10 C 200 I 10
P2 0 C 200 I 10 C 56
P3 100 C 300
P3 20 C 10 I 400
*/

struct task{
    char type; // CPU or I/O , types :- {C-> CPU , I->I/O} 
    int  time_req; // time required to complete this task in ms;
    task* next_task;

    //default constructor
    task(){};

    //parameterised constructor
    task(char a_type, int a_time,task* a_next_task);

};

struct context{
    int64_t ra;
    int64_t sp;
    int64_t pc;
    int64_t s0;
    int64_t s1;
    int64_t s2;
    int64_t s3;
    int64_t s4;
    int64_t s5;
    int64_t s6;
    int64_t s7;
    int64_t s8;
    int64_t s9;
    int64_t s10;
    int64_t s11;
};

struct process{
    int pid;                    // process id
    int arrival;                // arrival time
    task tasklisthead;          // first task
    bool running=false;         
    bool finished=false;
    int cpu_use = 0;
    task* curr_task = nullptr;  // which task is doing
    struct context*  registers; // content of registers
    int hart  = 0;
    //default constructor
    process(){};

    //parameterised constructor
    process(int a_pid,int a_arrival,task a_head);

    // save the context of process and restore the other
    void swtcontext(process* new_proc);

    // update context when singel process for context switch handel
    void savecontext();
    // print
    void print_process();
};


struct proc_smltr {
    int n; // number of process generate
    process* process_list;

    // constructor
    proc_smltr(int n);

    // destructor
    ~proc_smltr();

    // print process
    void print();

    void sort_procs();
};

process* new_process = nullptr;
#endif


