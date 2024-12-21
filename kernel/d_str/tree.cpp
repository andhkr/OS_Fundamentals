#include "ds.hpp"

// binary search tree

bst::bst(node* a_root){
    root = a_root;
    tree_lock = new ticket_lock();
}

void bst::insert(process* p) {
    node* prev_node = nullptr;
    bool left = false;

    tree_lock->lock(nullptr);
    node* curr_node = root;
    while(curr_node){
        prev_node = curr_node;
        if(curr_node->proc->arrival > p->arrival){
            curr_node = curr_node->left;
            left      = true;
        }else{
            curr_node = curr_node->right;
            left      = false;
        }
    }

    if(prev_node){
        if(left){
            prev_node->left = new node(p);
        }else{
            prev_node->right = new node(p);
        }
    }else{
        root =  new node(p);
    }

    tree_lock->unlock(nullptr);
}

bool bst::search(process* p) {
    bool is_find = false;
    while(root){
        if(root->proc == p){
            is_find = true;
            break;
        }
        root = (root->proc->arrival > p->arrival) ? root->left : root->right;
    }
    return is_find;
} 

void bst::remove(process* p) {
    tree_lock->lock(nullptr);

    node* curr_node = root;
    node* prev_node = nullptr;
    bool left = false;

    while(curr_node){
        if(curr_node->proc == p) break;
        prev_node = curr_node;
        if(curr_node->proc->arrival > p->arrival){
            curr_node = curr_node->left;
            left = true;
        }else{
            curr_node = curr_node->right;
            left = false;
        }
    }

    if(curr_node){
        // leaf node
        if(curr_node->left == nullptr && curr_node->right == nullptr){
            if(curr_node == root){
                root = nullptr;
            }
            else if(left){
                prev_node->left = nullptr;
            }else{
                prev_node->right = nullptr;
            }
        }
        // have one children
        else if(!curr_node->left || !curr_node->right ){
            node* newchild = (curr_node->left) ? (curr_node->left) : (curr_node->right);

            if(curr_node == root){
                root = newchild;
            }
            else if(left){
                prev_node->left = newchild;
            }else{
                prev_node->right = newchild;
            }
        }            
        // have two children
        /*
        replace the deleting node by its successor
        */
        else{
            // find successor -> smallest value in right subtree
            node* curr = curr_node->right;
            node* prev = curr_node;
            while(curr->left){
                prev = curr;
                curr = curr->left;
            }

            if(prev != curr_node){
                prev->left = curr->right;
                curr->right = curr_node->right;
            }
            // root(curr_node->right) itself is successor       
            curr->left = curr_node->left; 

            if(curr_node == root){
                root = curr;
            }
            else if(left){
                prev_node->left = curr;
            }else{
                prev_node->right = curr;
            }
        }
        
        delete curr_node;
        curr_node = nullptr;
    }else{
        fprintf(stderr,"Error: process %d not found\n",p->pid);
        exit(1);
    }
    tree_lock->unlock(nullptr);
}
