#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>

typedef struct
{
    pthread_mutex_t mtx;
    pthread_cond_t cond;
}Monitor;

Monitor *initMonitor(Monitor *monitor);
void enterMonitor(Monitor *monitor);
void exitMonitor(Monitor *monitor);
void wait(Monitor* monitor);
void signal(Monitor* monitor);
void signal_all(Monitor* monitor);
void destroyMonitor(Monitor *monitor);
