#include "semaphores.h"

void sem_create(sem_t *sem, int value)
{
    sem->value = value;
    sem->queue = NULL;
    sem->size = 0;
}

void sem_destroy(sem_t *sem)
{
    free(sem->queue);
    sem->value = 0;
    sem->queue = NULL;
    sem->size = 0;
}

void sem_up(sem_t *sem)
{
    if(sem->value == 1)
    {
        printf("Error: sem_up() called on a semaphore with value 1\n");
        return;
    }
    
    if(sem->size == 0)
    {
        sem->value++;
        return;
    }
    // make the first thread in the queue ready //
    sem->queue[0] = -1;

    shift_right(sem);
    sem->queue = (int *) realloc(sem->queue, (sem->size - 1) * sizeof(int));
    sem->size--;
}
    
void shift_right(sem_t *sem)
{
    for (int i = sem->size - 1; i >= 0; i--)
    {
        sem->queue[i + 1] = sem->queue[i];
    }
}

void sem_down(sem_t *sem, int thread_id)
{
    if(sem->value == 0)
    {
        sem->queue = (int *) realloc(sem->queue, (sem->size + 1) * sizeof(int));
        sem->queue[sem->size] = thread_id;
        sem->size++;
        // thread status to blocked //
        return;
    }
    sem->value--;
}

int main()
{
    
}