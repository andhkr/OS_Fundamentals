#include "lock.hpp"

ticket_lock::ticket_lock(){
        ticket = 0;
        turn   = 0;
}

// hold lock
void ticket_lock::lock(void*){
    int myturn = atomic_fetch_add(&ticket,1);
    while(myturn != turn){
        // yeild();
    }
    // every load stores should be visibel to all cpu cores
    // before entering in critical section
    asm volatile("mfence" ::: "memory");
}

void ticket_lock::unlock(void*){
    // every load stores should be visibel to all cpu cores
    // before leaving critical section
    asm volatile("mfence" ::: "memory");
    ++turn;
}


