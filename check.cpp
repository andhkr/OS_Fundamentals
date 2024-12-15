// #include<pthread.h>
// #include<signal.h>

// volatile int interrupt_num = 0;

// pthread_cond_t plic = PTHREAD_COND_INITIALIZER;
// pthread_cond_t intrhndl = PTHREAD_COND_INITIALIZER;
// pthread_cond_t sched = PTHREAD_COND_INITIALIZER;
// pthread_mutex_t   fplic = PTHREAD_MUTEX_INITIALIZER;
// pthread_mutex_t   fintr = PTHREAD_MUTEX_INITIALIZER;
// pthread_mutex_t   fsched = PTHREAD_MUTEX_INITIALIZER;

// void* plic(void* args){
//     for(;;){
//         while(interrupt_num<0);
//         if(interrupt_num == 0){
//             pthread_cond_signal(&sched);
//         }
//         while(interrupt_num!=100);
//         pthread_mutex_unlock(&fplic);
//     }
// }

// void* intrhndl(void* args){
//     pthread_mutex_lock(&fplic);
//     interrupt_num = 0;
    
// }

// void* sched(void* args){
//     interrupt_num = 100;
// }

// void* (*arr[])(void*) = {
//     plic,
//     intrhndl,
//     sched
// };

// int main(){
//     pthread_t thrd[4];
//     for (int i = 0;i<4;++i){
//         pthread_create(&thrd[i],NULL,arr[i],NULL);
//     }

//     for(int i = 0;i<4;++i){
//         pthread_join(thrd[i],NULL);
//     }
// }