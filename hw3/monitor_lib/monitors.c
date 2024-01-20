//
#include "monitors.h"

Monitor *initMonitor(Monitor *monitor)
{
    // monitor = (Monitor *) malloc(sizeof(Monitor));
    // if(monitor == NULL)
    // {
    //     printf("Malloc failed\n");
    // }
    if(pthread_mutex_init(&(monitor->mtx), NULL) != 0)
    {
        printf("ERROR on initialization\n");
    }
    pthread_cond_init(&(monitor->cond), NULL);

    return monitor;
}

void enterMonitor(Monitor *monitor)
{
    pthread_mutex_lock(&monitor->mtx);
}

void exitMonitor(Monitor *monitor)
{
    pthread_mutex_unlock(&monitor->mtx);
}

// Wait on the condition variable
void wait(Monitor* monitor) 
{
    if (pthread_cond_wait(&monitor->cond, &monitor->mtx) != 0)
    {
        printf("Wait failed\n");
    }
}

// Signal the condition variable
void signal(Monitor* monitor) 
{
    pthread_cond_signal(&monitor->cond);
}

void signal_all(Monitor* monitor) 
{
    pthread_cond_broadcast(&monitor->cond);
}

void destroyMonitor(Monitor *monitor)
{
    pthread_mutex_destroy(&monitor->mtx);
    pthread_cond_destroy(&monitor->cond);

    free(monitor);
}