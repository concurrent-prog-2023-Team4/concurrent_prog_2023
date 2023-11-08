// pthreads brokos
#include "hw1_1.h"

int main()
{
    pthread_t p1;

    int ret;

    ret = pthread_create(&p1, NULL, (void *(*)(void *))pipe_open, (void*)5);
    if(ret == 0)
        printf("Thread created succesfully\n");




    return 0;
}
