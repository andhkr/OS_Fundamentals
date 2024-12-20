#ifndef ROUNDROBIN_HPP
#define ROUNDROBIN_HPP

#include<iostream>
#include "d_str/ds.hpp"
#include <cassert>
#include <ctime>
#include <mutex>

#define timer_interrupt 10

struct RR{
    queue<process*> ready;
    bst blocked;
    int time_slice = timer_interrupt;
    //default constructor
    RR(){}

    RR(int slice);

    void proc_in(process* p);

    void schdintr_handler();

    void block(process* p);

    void Ready(process* p);

    void cpu_in(int hart,process* p);
};

void* scheduler(void*);
#endif
