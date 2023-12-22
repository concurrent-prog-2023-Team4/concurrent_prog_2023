#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <errno.h>
#include <pthread.h>
#include <stdlib.h>

typedef struct 
{
    int sem_id;
    int value;
    pthread_mutex_t mutex, safe_read;
    pthread_cond_t cond;
} mysem_t;

int mysem_create(mysem_t *s);
int mysem_init(mysem_t *s, int n);
int mysem_down(mysem_t *s);
int mysem_up(mysem_t *s);
int mysem_destroy(mysem_t *s);
