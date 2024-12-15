#ifndef ROUND_ROBIN_HPP
#define ROUND_ROBIN_HPP

#include<iostream>
#include "queue/proc_queue.cpp"
#include <cassert>
#include "proc.hpp"

#define timer_interrupt 10

struct RR{
    proc_queue ready;
    proc_queue blocked;
    int time_slice = timer_interrupt;
    //default constructor
    RR(){}

    RR(int slice);

    void proc_in(process* p);

    void schdintr_handler();

    void block(process* p);

    void Ready(process* p);
};

RR rnd_rbn = RR();
#endif
