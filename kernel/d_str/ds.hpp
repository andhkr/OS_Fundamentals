#ifndef DS_HPP
#define DS_HPP

#include "proc.hpp"
#include "lock.hpp"

class proc_queue{
    process* queue[max_procs];
    int head;
    int tail;
    int no_of_procs;
    teplock* qlock;

public:
    //default constructor
    proc_queue();

    // destructor
    ~proc_queue();

    //enqueue
    void enqueue(process* p);

    //dequeue
    process* dequeue();

    // number of process in queue
    int get_count();
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
