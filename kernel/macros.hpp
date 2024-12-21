#define cores 1             // number of cpus
#define max_procs             100 // maximum process system can handel
#define max_task_per_process  20
#define max_arrival_time_unit 100
#define max_time_per_task     80

#define number_of_procs 1     // processes simulated by proc_simulator
extern int haveinterrupt;     // interrupts bit per core
extern int coresinuse;        // used by scheduler 
#define setbit(x,y)((y) = ((y) | (1<<(x))))
#define unsetbit(x,y)((y) = ((y) & ~(1<<(x))))
#define bitvalue(x,y)(((y) & (1<<(x))) == 1)