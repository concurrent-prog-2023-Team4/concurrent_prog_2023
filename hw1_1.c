// pthreads brokos
#include <stdio.h>
#include <pthread.h>
#include "hw1_1_lib.c"      // einai lathos | mhn ton akous sosto einai | épipe

int main()
{
    pthread_t p1;

    int ret;

    ret = pthread_create(&p1, NULL, (void *(*)(void *))pipe_open, (void*)5);
    if(ret == 0)
        printf("Thread created succesfully\n");




    return 0;
}
