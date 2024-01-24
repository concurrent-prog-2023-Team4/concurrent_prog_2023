#include "all.h"

int mycoroutines_init(co_t *main)
{
    // Initialize the main coroutine
    getcontext(main->cot);
    (*main).cot->uc_link = 0;
    (*main).cot->uc_stack.ss_sp = (*main).stack;
    (*main).cot->uc_stack.ss_size = STACK_SIZE;
    (*main).cot->uc_stack.ss_flags = 0;
    (*main).next_cot = NULL;

    return 0;
}

int mycoroutines_create(co_t *coroutine, void (*routine)(void *), void *arg)
{
    // Initialize the coroutine coroutine
    if(coroutine == NULL)
    {
        coroutine = (co_t *) malloc(sizeof(co_t));
    }
    coroutine->cot = (ucontext_t *) malloc(sizeof(ucontext_t));
    getcontext(coroutine->cot);
    (*coroutine).cot->uc_link = 0;
    (*coroutine).cot->uc_stack.ss_sp = (*coroutine).stack;
    (*coroutine).cot->uc_stack.ss_size = STACK_SIZE;
    (*coroutine).cot->uc_stack.ss_flags = 0;
    makecontext(coroutine->cot, (void (*)(void))routine, 1, arg);

    return 0;
}

int mycoroutines_switchto(co_t *thread1)
{
    // Save the current context to thread1 and switch to thread2
    if (swapcontext(thread1->cot, thread1->next_cot) == -1) 
    {
        printf("Error occurred during context switch\n");
        return -1; // Error occurred during context switch
    }

    return 0;
}

int mycoroutines_destroy(co_t *thread)
{
    // Destroy the thread coroutine
    (*thread).cot->uc_link = 0;
    (*thread).cot->uc_stack.ss_sp = NULL;
    (*thread).cot->uc_stack.ss_size = 0;
    (*thread).cot->uc_stack.ss_flags = 0;

    return 0;
}


sem_t *sem_create(sem_t *sem, int value)
{
    if(sem == NULL)
    {
        sem = (sem_t *) malloc(sizeof(sem_t));
    }
    sem->value = value;
    sem->queue = NULL;
    sem->size = 0;

    return sem;
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
    sem_down(mtx, thread->id);

    thread->state = READY;
    thread->finish = NULL;
    thread->finish = sem_create(thread->finish, 0);
    thread->id = thread_ids;
    thread_ids++;
    mycoroutines_create(thread->coroutine, func, arg);

    sem_up(mtx);
}

void mythreads_join(thr_t *thread)
{
    // Join the thread coroutine //
    sem_down(thread->finish, thread->id);
}

void mythread_destroy(thr_t *thread)
{
    sem_down(mtx, thread->id);

    mycoroutines_destroy(thread->coroutine);
    thread->state = BLOCKED;
    sem_destroy(thread->finish);
    thread->id = -1;

    sem_up(thread->finish); // inform that thread finished //

    sem_up(mtx);
}

void mythtreads_init()
{
    mtx = NULL;
    mtx = sem_create(mtx, 1);
    thread_ids = 0;
    set_alarm(500);  // 500 useconds //
}

void find_thread(int thread_id, int *pos)
{
    int i;

    *pos = -1;
    for(i = 0; threads_array[i].id != -1; i++)
    {
        if(threads_array[i].id == thread_id)
        {
            
            *pos = i;
            break;
        }
    }

}

void handle_alarm(int signum)
{
    int i;

    for(i = current_thread + 1; i == current_thread; i++)
    {
        if(threads_array[i].id == -1)
        {
            i = -1;
            continue;
        }
        if(threads_array[i].state == READY)
        {
            threads_array[current_thread].coroutine->next_cot = threads_array[i].coroutine->cot;
            mycoroutines_switchto(threads_array[current_thread].coroutine);
            break;
        }
    }
}

// Function to set an alarm for a specified number of seconds
void set_alarm(int seconds) 
{
    // Set the alarm signal handler
    signal(SIGALRM, handle_alarm);

    // Set the alarm to trigger after the specified number of seconds
    alarm(seconds);

    // Sleep to allow the alarm to trigger
    usleep(seconds);
}


void *thread_func(void *arg)
{
    int *curr_id = (int *) arg;
    // Print the thread ID
    printf("Thread created\n");

    // Destroy the current thread
    mythread_destroy(& (threads_array[*curr_id]) );

    return NULL;
}

int main()
{
    int i;
    // Initialize the main thread
    i = 0;
    mythtreads_init();

    threads_array = (thr_t *) malloc(4 * sizeof(thr_t));

    // Create the first thread
    mythreads_create(&threads_array[0], thread_func, &i);

    i++;
    // Create the second thread
    mythreads_create(&threads_array[1], thread_func, &i);

    threads_array[2].id = -1;

    // Join the first thread
    mythreads_join(&threads_array[0]);

    // Join the second thread
    mythreads_join(&threads_array[1]);

    return 0;
}


