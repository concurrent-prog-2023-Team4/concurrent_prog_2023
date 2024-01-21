//
#include <stdio.h>
#include <stdlib.h>

struct semaphore
{
    unsigned short value;
    int *queue; // -1 terminated //
    int size;
};
typedef struct semaphore sem_t;

void sem_create(sem_t *sem, int value);
void sem_destroy(sem_t *sem);
void sem_up(sem_t *sem);
void sem_down(sem_t *sem, int thread_id);
void shift_right(sem_t *sem);


