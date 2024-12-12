#include <string>
#include <iostream>

#define max_task_per_process  20
#define max_arrival_time_unit 100
#define max_time_per_task     1000

/*
P1 5 C 20 I 10 C 200 I 10
P2 0 C 200 I 10 C 56
P3 100 C 300
P3 20 C 10 I 400
*/

// forward declaration
class process;
class proc_smltr;

class task{
    char type; // CPU or I/O , types :- {C-> CPU , I->I/O} 
    int  time_req; // time required to complete this task in ms;
    task* next_task;

public:
    //default constructor
    task(){}

    //parameterised constructor
    task(char a_type, int a_time,task* a_next_task)
    :type(a_type),time_req(a_time),next_task(a_next_task)
    {}

    // freind classes
    friend process;
    friend proc_smltr;
};

class process{
    int pid; // process id
    int arrival; // arrival time
    task tasklisthead; // first task

public:
    //default constructor
    process(){}

    //parameterised constructor
    process(int a_pid,int a_arrival,task a_head)
    :pid(a_pid),arrival(a_arrival),tasklisthead(a_head)
    {}

    // print
    void print_process(){
        std::cout<<"P"<<pid<<" "<<arrival<<" ";
        task* curr = &tasklisthead;
        while(curr){
            std::cout<<curr->type<<" "<<curr->time_req<<" ";
            curr = curr->next_task;
        }
        std::cout<<std::endl;
    }

    // freind class
    friend proc_smltr;
};


class proc_smltr{
public:    
    // constructor
    proc_smltr(int n){
        this->n = n;
        process_list = new process[n];

        for(int i = 0;i<n;++i){
            process_list[i].pid = i;
            int arrival = rand()%(max_arrival_time_unit*(i+1));
            process_list[i].arrival = arrival;

            // tasklist
            int number_of_task = (1+rand()%max_task_per_process);
            int job_lenght     = (1+rand())%max_time_per_task;
            process_list[i].tasklisthead = task('C',job_lenght,NULL);

            task* curr_proc = &process_list[i].tasklisthead;

            for(int j = 1;j<number_of_task;++j){
                int i_type = rand()%2;
                char type  = 'C';
                if(i_type) type = 'I';

                job_lenght     = (1+rand())%max_time_per_task;
                curr_proc->next_task = new task(type,job_lenght,NULL);
                curr_proc = curr_proc->next_task;
            }
        }
    }

    // print process
    void print(){
        for(int i = 0;i<n;++i){
            process_list[i].print_process();
        }
    }

private:    
    int n; // number of process generate
    process* process_list;
};

int main(){
    proc_smltr processes = proc_smltr(4);
    processes.print();
}