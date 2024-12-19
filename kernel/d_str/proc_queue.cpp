#include "ds.hpp"


//default constructor
proc_queue::proc_queue(){
    head = 0;
    tail = 0;
    no_of_procs = 0;
    qlock       = new ticket_lock();
}

// destructor
proc_queue::~proc_queue(){
    delete qlock;
    qlock = nullptr;
}

//enqueue
void proc_queue::enqueue(process* p){
    qlock->lock(nullptr);
    queue[tail] = p;
    tail = (tail+1)%max_procs;
    ++no_of_procs;
    qlock->lock(nullptr);
}

//dequeue
process* proc_queue::dequeue(){
    qlock->lock(nullptr);
    process* p = queue[head];
    head = (head+1)%max_procs;
    --no_of_procs;
    qlock->lock(nullptr);
    return p;
}

// number of process in queue
int proc_queue::get_count(){
    return no_of_procs;
}
