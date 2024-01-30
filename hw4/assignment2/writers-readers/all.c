#include "all.h"
// int finish;
int current_thread;
int thread_ids;
thr_t *threads_array;
sem_t *lib_mtx;
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

int sem_up(sem_t *sem)
{
    int thread_pos;
    sigset_t new_mask;
    
    sigemptyset(&new_mask);
    sigaddset(&new_mask, SIGALRM);
    sigprocmask(SIG_BLOCK, &new_mask, NULL);

    if(sem->value == 1)
    {
        printf("Error: sem_up() called on a semaphore with value 1\n");
        return 0;   // lost up //
    }
    
    if(sem->size == 0)
    {
        sem->value++;
        sigprocmask(SIG_UNBLOCK, &new_mask, NULL);
        return 1;
    }
    // make the first thread in the queue ready //
    find_thread(sem->queue[0], &thread_pos);
    if(thread_pos == -1)
    {
        printf("Thread not found!\n");
        sigprocmask(SIG_UNBLOCK, &new_mask, NULL);
        return -2; // thread not found //
    }
    threads_array[thread_pos].state = READY;
    sem->queue[0] = 0;

    shift_left(sem);
    sem->queue = (int *) realloc(sem->queue, (sem->size - 1) * sizeof(int));
    sem->size--;

    sigprocmask(SIG_UNBLOCK, &new_mask, NULL);

    return 1;   // succeeded //
}
    
void shift_left(sem_t *sem)
{
    for (int i = 0; i < sem->size - 1; i++)
    {
        sem->queue[i] = sem->queue[i + 1];
    }
}


int sem_down(sem_t *sem)
{
    int thread_pos;
    int i;
    int temp;
    sigset_t new_mask;
    int found_avaialble_thread = 0;
    int thread_id;
    
    thread_id = current_thread; 

    sigemptyset(&new_mask);
    sigaddset(&new_mask, SIGALRM);
    sigprocmask(SIG_BLOCK, &new_mask, NULL);

    if(sem->value == 0)
    {
        sem->queue = (int *) realloc(sem->queue, (sem->size + 1) * sizeof(int));
        sem->queue[sem->size] = threads_array[current_thread].id;   // we keep ids in queue //
        sem->size++;
        // thread status to blocked //
        // find_thread(thread_id, &thread_pos);
        threads_array[current_thread].state = BLOCKED;

        for(i = current_thread + 1; i != current_thread; i++)
        {
            if(threads_array[i].id == 0)
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
                found_avaialble_thread = 1;
                mycoroutines_switchto(threads_array[temp].coroutine);
                break;
            }
        }

        if(found_avaialble_thread == 0)
            printf("no threads available\n");

        sigprocmask(SIG_UNBLOCK, &new_mask, NULL);
        return 1;   // if it value was 0 and we blocked someone //
    }
    sem->value--;
    
    sigprocmask(SIG_UNBLOCK, &new_mask, NULL);

    return 1;   // if it value was 1 //
}

void mythreads_create(thr_t *thread, void (*func)(void *), void *arg)
{
    // Initialize the thread coroutine //
    sem_down(lib_mtx);

    thread->state = READY;
    thread->finish = NULL;
    thread->finish = sem_create(thread->finish, 0);
    thread->id = thread_ids;
    thread_ids++;
    mycoroutines_create(&(thread->coroutine), func, arg);

    sem_up(lib_mtx);
}

void mythreads_join(thr_t *thread)
{
    // Join the thread coroutine //
    if(thread->finish == NULL)
    {
        printf("Finish is NULL\n");
        return;
    }
    sem_down(thread->finish);
}

void mythread_destroy(thr_t *thread)
{
    int i, j;
    thr_t copy_thread;

    sem_down(lib_mtx);

    for(i = 0; threads_array[i].id != 0; i++)
    {
        if(threads_array[i].id == thread->id)
        {
            // copy_thread = threads_array[i];
            mycoroutines_destroy(threads_array[i].coroutine);
            sem_destroy(threads_array[i].finish);
            threads_array[i].id = -1;

            break;
        }
    }
    // for(j = i; threads_array[j].id != 0; j++)
    // {
    //     threads_array[j] = threads_array[j + 1];
    // }
    // threads_array[j+1] = copy_thread;
     // threads_array = (thr_t *) realloc(threads_array, (j + 1) * sizeof(thr_t));

    sem_up(lib_mtx);
}

void mythtreads_init()
{
    thread_ids = 1;
    // set_alarm(50);  // 500 useconds //
    timer_init();
}

void find_thread(int thread_id, int *pos)
{
    int i;

    *pos = -1;
    for(i = 0; threads_array[i].id != 0; i++)
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
    int found_avaialble_thread = 0;

    for(i = 0; threads_array[i].id != 0; i++)
    {
        if(threads_array[i].id != -1)
        {
            if(threads_array[i].state == FINISH)
            {
                printf("Thread %d is exiting\n", threads_array[i].id);
                mythread_destroy(&threads_array[i]);
            }
        }
    }

    for(i = current_thread + 1; i != current_thread; i++)
    {
        if(threads_array[i].id == -1)
        {
            continue;
        }
        if(threads_array[i].id == 0)
        {
            i = -1;
            continue;
        }
        if(threads_array[i].state == YIELD)
        {
            printf("Found yield on a thread\n");
            threads_array[i].state = READY;
            continue;
        }
        if(threads_array[i].state == READY)
        {
            found_avaialble_thread = 1;
            threads_array[current_thread].coroutine->next_cot = threads_array[i].coroutine->cot;
            // printf ("curr thread is %d\n", i);
            temp = current_thread;
            current_thread = i;
            mycoroutines_switchto(threads_array[temp].coroutine);
            
            break;
        }
    }

    // check if the first one is available //
    if(found_avaialble_thread == 0)
    {
        // printf("Scheduler did not found threads available\n");
        if(threads_array[i].id != -1 && threads_array[i].id != 0)
        {
            if(threads_array[i].state == READY)
            {
                found_avaialble_thread = 1;
                threads_array[current_thread].coroutine->next_cot = threads_array[i].coroutine->cot;
                // printf ("curr thread is %d\n", i);
                temp = current_thread;
                current_thread = i;
                mycoroutines_switchto(threads_array[temp].coroutine);
                
            }
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
    t.it_value.tv_usec = 50000;
    t.it_interval.tv_sec = 0;
    t.it_interval.tv_usec = 50000;


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

    while(1)
    {
        printf("Thread func 1\n");
        sem_down(threads_array[*curr_id].finish);
    }

    return NULL;
}

void *thread_func_2(void *arg)
{
    int *curr_id = (int *) arg;
    printf("Thread created %d\n", *curr_id);

    sem_up(threads_array[1].finish);
    while(1)
    {
        // sem_down(threads_array[*curr_id].finish);
        // sem_down(threads_array[*curr_id].finish);
    }


    return NULL;
}

double get_current_time()
{
    return (double) time(NULL);
}

void custom_sleep(double seconds)
{
    double start_time = get_current_time();
    double end_time = start_time + seconds;

    while(get_current_time() < end_time)
    {
        // do nothing //
    }
}

void mythread_yield()
{
    threads_array[current_thread].state = YIELD;

    while(threads_array[current_thread].state != READY)
    {

    }
}

void mythread_exit()
{
    threads_array[current_thread].state = FINISH;

    sem_up(threads_array[current_thread].finish);

    while(1)
    {
        
    }
}