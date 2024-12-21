#ifndef IO_HPP
#define IO_HPP
#include "interrupthndl.hpp"
#include <pthread.h>

extern pthread_cond_t iocond;

// iorequest receiver
class ioreqrec{
public:    
    queue<interrupt_info*> ioqueue;

    void rec(interrupt_info* info){
        ioqueue.enqueue(info);
        pthread_cond_signal(&iocond);
    }

    void service_request(){
        
        while(ioqueue.no_of_procs){
            interrupt_info* old_info = ioqueue.dequeue();
            // std::cout<<"request servicing of p "<<old_info->p->pid<<std::endl;
            int iotime = old_info->iotime;
            long counter = 0;
            clock_t start = clock();
            while(((clock()-start)*1000/CLOCKS_PER_SEC)<iotime)counter++;
            interrupt_info* info = new interrupt_info();
            info->cause  = i_o_finished;
            info->hart   = cores;     // cpu reserved for os
            info->p      = old_info->p;
            // std::cout<<"request servicing of p "<<old_info->p->pid<<std::endl;
            interrupt_handler(info);
        }
    }
};
#endif