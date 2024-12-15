#include <atomic>
#include <iostream>

/*
An Abstract class for lock(apnalock) so that can make multiple
locks for different scenario with same interface.
And in case of any change in lock for efficiency: I have to only
change make change in concrete class for lock and rest of things will be
almost same.
*/

// teplock:Three Easy Pieces lock -> to credict the book from where i learn

struct teplock{
    virtual void lock(void*) = 0;
    virtual void unlock(void*) = 0;

    // virtual destructor
    virtual ~teplock(){};
};


struct ticket_lock:teplock{
    
    std::atomic<int> ticket;
    int turn;

    ticket_lock(){
        ticket = 0;
        turn   = 0;
    }

    // hold lock
    void lock(void*) override{
        int myturn = atomic_fetch_add(&ticket,1);
        while(myturn != turn){
            // yeild();
        }
        // every load stores should be visibel to all cpu cores
        // before entering in critical section
        asm volatile("mfence" ::: "memory");
    }

    void unlock(void*) override{
        // every load stores should be visibel to all cpu cores
        // before leaving critical section
        asm volatile("mfence" ::: "memory");
        ++turn;
    }
};

