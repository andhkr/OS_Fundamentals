#include "kernel/proc.hpp"

class proc_queue{
    process* queue[max_procs];
    int head;
    int tail;
    int no_of_procs;

public:
    //default constructor
    proc_queue(){
        head = 0;
        tail = 0;
        no_of_procs = 0;
    }

    //enqueue
    void enqueue(process* p){
        queue[tail] = p;
        tail = (tail+1)%max_procs;
        ++no_of_procs;
    }

    //dequeue
    process* dequeue(){
        process* p = queue[head];
        head = (head+1)%max_procs;
        --no_of_procs;
        return p;
    }

    // number of process in queue
    int get_count(){
        return no_of_procs;
    }
};