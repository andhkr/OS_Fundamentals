#include "interrupthndl.hpp"

void iorequest(int hart,int time,process* p){
    clock_t start = clock();
    volatile long counter = 0;
    while(((clock()-start)*1000/CLOCKS_PER_SEC)!=time)counter++;
    interrupt_info* info = new interrupt_info();
    info->cause  = i_o_finished;
    info->hart   = hart;
    info->p      = p;
    interrupt_handler(info);
}