#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <ucontext.h>
#include <sys/time.h>
#include <time.h>

// #define MY_CUSTOM_SIGNAL (SIGRTMIN + 1)

/* -----------------Coroutines Library -------------------------------------*/

#define STACK_SIZE 16384
struct coroutine
{
    ucontext_t *cot;
    ucontext_t *next_cot;
    char stack[STACK_SIZE];
};
typedef struct coroutine co_t;

int mycoroutines_init(co_t *coroutines);
int mycoroutines_create(co_t **coroutines, void (*func)(void *), void *arg);
int mycoroutines_switchto(co_t *coroutines);
int mycoroutines_destroy(co_t *coroutines);

/* ----------------------------------------------------------------------*/

/* -----------------Semaphore Library -------------------------------------*/
struct semaphore
{
    unsigned short value;
    int *queue; // -1 terminated //
    int size;
};
typedef struct semaphore sem_t;

sem_t *sem_create(sem_t *sem, int value);
int sem_destroy(sem_t *sem);
int sem_up(sem_t *sem);
int sem_down(sem_t *sem);
void shift_left(sem_t *sem);

/* ----------------------------------------------------------------------*/

/* -----------------Threads Library -------------------------------------*/
struct my_threads
{
    int id;
    enum {READY, BLOCKED, FINISH, YIELD} state;
    co_t *coroutine;
    sem_t *finish;
};
typedef struct my_threads thr_t;

extern int current_thread;     // index of current thread in threads_array //
extern int thread_ids;
extern thr_t *threads_array;   // -1 terminated array of threads //

int mythtreads_init();
int mythreads_create(thr_t *thread, void (*func)(void *), void *arg);
int mythreads_join(thr_t *thread);
int mythread_destroy(thr_t *thread);
int mythread_yield();
int mythread_exit();
void find_thread(int thread_id, int *pos);

/* ----------------------------------------------------------------------*/

/* ----------------- Scheduler -------------------------------------*/
extern sem_t *lib_mtx; // we need to init //
extern int finish;

void scheduler(int signum);
void set_alarm(int seconds);
int timer_init();
double get_current_time();
void custom_sleep(double seconds);
/* ----------------------------------------------------------------------*/