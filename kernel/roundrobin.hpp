#ifndef ROUNDROBIN_HPP
#define ROUNDROBIN_HPP

#include<iostream>
#include "d_str/ds.hpp"
#include <cassert>

#define timer_interrupt 10

struct RR{
    proc_queue ready;
    bst blocked;
    int time_slice = timer_interrupt;
    //default constructor
    RR(){}

    RR(int slice);

    void proc_in(process* p);

    void schdintr_handler();

    void block(process* p);

    void Ready(process* p);
};

extern RR rnd_rbn;

void scheduler(int slice);
#endif
