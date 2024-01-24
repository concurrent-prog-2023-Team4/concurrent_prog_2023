#include "all.h"
int finish;


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

int mycoroutines_create(co_t **coroutine, void (*routine)(void *), void *arg)
{
    // Initialize the coroutine coroutine
    if(*coroutine == NULL)
    {
        *coroutine = (co_t *) malloc(sizeof(co_t));
    }
    (*coroutine)->cot = (ucontext_t *) malloc(sizeof(ucontext_t));
    getcontext((*coroutine)->cot);
    (*coroutine)->cot->uc_link = 0;
    (*coroutine)->cot->uc_stack.ss_sp = (*coroutine)->stack;
    (*coroutine)->cot->uc_stack.ss_size = STACK_SIZE;
    (*coroutine)->cot->uc_stack.ss_flags = 0;
    makecontext((*coroutine)->cot, (void (*)(void))routine, 1, arg);

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

    shift_left(sem);
    sem->queue = (int *) realloc(sem->queue, (sem->size - 1) * sizeof(int));
    sem->size--;

    sigprocmask(SIG_SETMASK, &old_mask, NULL);
}
    
void shift_left(sem_t *sem)
{
    for (int i = 0; i < sem->size - 1; i++)
    {
        sem->queue[i] = sem->queue[i + 1];
    }
}


void sem_down(sem_t *sem, int thread_id)
{
    int thread_pos;
    int i;
    int temp;
    sigset_t new_mask, old_mask;

    sigemptyset(&new_mask);
    sigaddset(&new_mask, SIGALRM);
    sigprocmask(SIG_BLOCK, &new_mask, NULL);

    if(sem->value == 0)
    {
        sem->queue = (int *) realloc(sem->queue, (sem->size + 1) * sizeof(int));
        sem->queue[sem->size] = thread_id;
        sem->size++;
        // thread status to blocked //
        find_thread(thread_id, &thread_pos);
        threads_array[thread_pos].state = BLOCKED;

        for(i = current_thread + 1; i != current_thread; i++)
        {
            if(threads_array[i].id == -1)
            {
                i = -1;
                continue;
            }
            if(threads_array[i].state == READY)
            {
                threads_array[current_thread].coroutine->next_cot = threads_array[i].coroutine->cot;
                printf("From down: switch to %d\n", i);
                temp = current_thread;
                current_thread = i;
                mycoroutines_switchto(threads_array[temp].coroutine);
                break;
            }
        }

        printf("no threads available\n");

        return;
    }
    sem->value--;
    
    sigprocmask(SIG_UNBLOCK, &new_mask, NULL);
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
    mycoroutines_create(&(thread->coroutine), func, arg);

    sem_up(mtx);
}

void mythreads_join(thr_t *thread)
{
    // Join the thread coroutine //
    sem_down(thread->finish, 0);
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
    thread_ids = 0;
    // set_alarm(50);  // 500 useconds //
    timer_init();
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
    int temp;

    for(i = current_thread + 1; i != current_thread; i++)
    {
        if(threads_array[i].id == -1)
        {
            i = -1;
            continue;
        }
        if(threads_array[i].state == READY)
        {
            threads_array[current_thread].coroutine->next_cot = threads_array[i].coroutine->cot;
            printf ("curr thread is %d\n", i);
            temp = current_thread;
            current_thread = i;
            mycoroutines_switchto(threads_array[temp].coroutine);
            
            break;
        }
    }
}

// // Function to set an alarm for a specified number of seconds
// void set_alarm(int seconds) 
// {
//     // Set the alarm signal handler
//     signal(SIGALRM, handle_alarm);

//     // Set the alarm to trigger after the specified number of seconds
//     ualarm(seconds, 0);

//     // Sleep to allow the alarm to trigger
//     // usleep(seconds);
// }

void timer_init()
{
    struct itimerval t = {{0}};
    struct sigaction act = {{0}};
    int sig_val;

    act.sa_handler = handle_alarm;
    act.sa_flags = SA_RESTART;

    //alarm every 0.5 seconds
    t.it_value.tv_sec = 0;
    t.it_value.tv_usec = 500000;
    t.it_interval.tv_sec = 0;
    t.it_interval.tv_usec = 500000;


    sig_val = sigaction(SIGALRM, &act, NULL);
    if(sig_val == -1)
    {
        perror("sigaction error");
        return;
    }

    setitimer(ITIMER_REAL,&t,NULL);
}

void *thread_func(void *arg)
{
    int *curr_id = (int *) arg;
    printf("Thread created %d\n", *curr_id);
    //while(1)
    //{
        // if(finish == 1)
        // {
        //     break;
        // }
    //    printf("in thread %d\n", *curr_id);

    //}
    // Print the thread ID

    sleep(10);

    // Destroy the current thread
    // mythread_destroy(& (threads_array[*curr_id]));

    return NULL;
}

int main()
{
    int i;
    // Initialize the main thread
    i = 0;
    int j = 2;

    mtx = NULL;
    mtx = sem_create(mtx, 1);

    signal(MY_CUSTOM_SIGNAL, handle_alarm);

    threads_array = (thr_t *) malloc(4 * sizeof(thr_t));

    mythreads_create(&threads_array[0],(void*) main, &i);
 
    i++;

    // Create the first thread
    mythreads_create(&threads_array[1], (void*) thread_func, &j);

    // Create the second thread
    mythreads_create(&threads_array[2], (void*) thread_func, &i);

    // printf("Threads created\n");

    threads_array[3].id = -1;

    mythtreads_init();

    sleep(5);

    // finish = 1;
    // Join the first thread
    mythreads_join(&threads_array[1]);

    // Join the second thread
    mythreads_join(&threads_array[2]);

    return 0;
}


