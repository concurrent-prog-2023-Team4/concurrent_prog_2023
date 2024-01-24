#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <ucontext.h>

struct semaphore
{
    unsigned short value;
    int *queue; // -1 terminated //
    int size;
};
typedef struct semaphore sem_t;

sem_t *sem_create(sem_t *sem, int value);
void sem_destroy(sem_t *sem);
void sem_up(sem_t *sem);
void sem_down(sem_t *sem, int thread_id);
void shift_right(sem_t *sem);

#define STACK_SIZE 16384

struct coroutine
{
    ucontext_t *cot;
    ucontext_t *next_cot;
    char stack[16384];
};
typedef struct coroutine co_t;

int mycoroutines_init(co_t *coroutines);
int mycoroutines_create(co_t *coroutines, void (*func)(void *), void *arg);
int mycoroutines_switchto(co_t *coroutines);
int mycoroutines_destroy(co_t *coroutines);

void handle_alarm(int signum);
void set_alarm(int seconds);

int current_thread;     // index of current thread in threads_array //
void *thread_func(void *arg);


struct my_threads
{
    int id;
    enum {READY, BLOCKED} state;
    co_t *coroutine;
    sem_t *finish;
};
typedef struct my_threads thr_t;

void mythtreads_init();
void mythreads_create(thr_t *thread, void (*func)(void *), void *arg);
void mythreads_join(thr_t *thread);
void mythread_destroy(thr_t *thread);
void find_thread(int thread_id, int *pos);

int thread_ids;
thr_t *threads_array;   // -1 terminated array of threads //

sem_t *mtx; // we need to init //
