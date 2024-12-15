#include "kernel/proc.hpp"

class proc_queue{
    process* queue[max_procs];
    int head;
    int tail;

public:
    //default constructor
    proc_queue(){
        head = 0;
        tail = 0;
    }

    //enqueue
    void enqueue(process* p){
        queue[tail] = p;
        tail = (tail+1)%max_procs;
    }

    //dequeue
    process* dequeue(){
        process* p = queue[head];
        head = (head+1)%max_procs;
        return p;
    }
};