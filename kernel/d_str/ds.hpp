#ifndef DS_HPP
#define DS_HPP

#include "proc.hpp"
#include "lock.hpp"

template<typename T>
class queue{
public:
    T  entityqueue[max_procs];
    int head;
    int tail;
    int no_of_procs;
    teplock* qlock;

    //default constructor
    queue(){
        head = 0;
        tail = 0;
        no_of_procs = 0;
        qlock       = new ticket_lock();
    }

    // destructor
    ~queue(){
        delete qlock;
        qlock = nullptr;
    }

    //enqueue
    void enqueue(T p){
        qlock->lock(nullptr);
        entityqueue[tail] = p;
        tail = (tail+1)%max_procs;
        ++no_of_procs;
        qlock->unlock(nullptr);
    }

    //dequeue
    T  dequeue(){
        qlock->lock(nullptr);
        T p = entityqueue[head];
        head = (head+1)%max_procs;
        --no_of_procs;
        qlock->unlock(nullptr);
        return p;
    }

    // number of entity in queue
    int get_count(){
        qlock->lock(nullptr);
        int count = no_of_procs;
        qlock->unlock(nullptr);
        return count;
    }
};

// trees
#include <cassert>

struct node{
    node* left;
    node* right;
    process* proc;

    node(){};
    node(process* p):left(nullptr),right(nullptr),proc(p){}
    node(node* a_left,node* a_right,process* p):left(a_left),right(a_right),proc(p){}
};

struct tree{
    node* root;
    teplock* tree_lock;

    virtual ~tree(){};

    virtual void insert(process* p)=0;
    virtual bool search(process* p)=0;
    virtual void remove(process* p)=0;
};

// binary search tree
struct bst:tree{

    bst(){
        root = nullptr;
        tree_lock = new ticket_lock();
    }

    bst(node* a_root);

    void insert(process* p) override;

    bool search(process* p) override;

    void remove(process* p) override;
};

#endif
