//
#include "my_threads.h"

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
    int thread_pos;
    sigset_t new_mask, old_mask;
    
    sigemptyset(&new_mask);
    sigaddset(&new_mask, SIGALRM);
    sigprocmask(SIG_BLOCK, &new_mask, &old_mask);

    if(sem->value == 1)
    {
        printf("Error: sem_up() called on a semaphore with value 1\n");
        return;
    }
    
    if(sem->size == 0)
    {
        sem->value++;
        sigprocmask(SIG_SETMASK, &old_mask, NULL);
        return;
    }
    // make the first thread in the queue ready //
    find_thread(sem->queue[0], &thread_pos);
    if(thread_pos == -1)
    {
        printf("Thread not found!\n");
        sigprocmask(SIG_SETMASK, &old_mask, NULL);
        return;
    }
    threads_array[thread_pos].state = READY;
    sem->queue[0] = -1;

    shift_right(sem);
    sem->queue = (int *) realloc(sem->queue, (sem->size - 1) * sizeof(int));
    sem->size--;

    sigprocmask(SIG_SETMASK, &old_mask, NULL);
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
    int thread_pos;
    sigset_t new_mask, old_mask;

    sigemptyset(&new_mask);
    sigaddset(&new_mask, SIGALRM);
    sigprocmask(SIG_BLOCK, &new_mask, &old_mask);

    if(sem->value == 0)
    {
        sem->queue = (int *) realloc(sem->queue, (sem->size + 1) * sizeof(int));
        sem->queue[sem->size] = thread_id;
        sem->size++;
        // thread status to blocked //
        find_thread(thread_id, &thread_pos);
        threads_array[thread_pos].state = BLOCKED;

        sigprocmask(SIG_SETMASK, &old_mask, NULL);

        return;
    }
    sem->value--;
    sigprocmask(SIG_SETMASK, &old_mask, NULL);

    signal(SIGUSR1, handle_alarm);
}



void mythreads_create(thr_t *thread, void (*func)(void *), void *arg)
{
    // Initialize the thread coroutine //
    sem_down(&mtx, thread->id);
    mycoroutines_create(thread->coroutine, func, arg);

    thread->state = READY;
    sem_create(thread->finish, 0);
    thread->id = thread_ids;
    thread_ids++;

    sem_up(&mtx);
}

void mythreads_join(thr_t *thread)
{
    // Join the thread coroutine //
    sem_down(thread->finish, thread->id);
}

void mythread_destroy(thr_t *thread)
{
    sem_down(&mtx, thread->id);

    mycoroutines_destroy(thread->coroutine);
    thread->state = BLOCKED;
    sem_destroy(thread->finish);
    thread->id = -1;

    sem_up(thread->finish); // inform that thread finished //

    sem_up(&mtx);
}

void mythtreads_init()
{
    sem_create(&mtx, 1);
    thread_ids = 0;
    set_alarm(500);  // 500 useconds //
}

void find_thread(int thread_id, int *pos)
{
    int i;

    i = -1;
    for(i = 0; threads_array[i].id != -1; i++)
    {
        if(threads_array[i].id == thread_id)
        {
            
            pos = i;
            break;
        }
    }

}



