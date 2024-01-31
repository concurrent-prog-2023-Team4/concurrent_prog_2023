#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <ucontext.h>
#include <sys/time.h>
#include <time.h>

#define MY_CUSTOM_SIGNAL (SIGRTMIN + 1)

struct semaphore
{
    unsigned short value;
    int *queue; // -1 terminated //
    int size;
};
typedef struct semaphore sem_t;

sem_t *sem_create(sem_t *sem, int value);
void sem_destroy(sem_t *sem);
int sem_up(sem_t *sem);
int sem_down(sem_t *sem);
void shift_left(sem_t *sem);

#define STACK_SIZE 16384

struct coroutine
{
    ucontext_t *cot;
    ucontext_t *next_cot;
    char stack[16384];
};
typedef struct coroutine co_t;

int mycoroutines_init(co_t *coroutines);
int mycoroutines_create(co_t **coroutines, void (*func)(void *), void *arg);
int mycoroutines_switchto(co_t *coroutines);
int mycoroutines_destroy(co_t *coroutines);

void handle_alarm(int signum);
void set_alarm(int seconds);

extern int current_thread;     // index of current thread in threads_array //
void *thread_func(void *arg);


struct my_threads
{
    int id;
    enum {READY, BLOCKED, FINISH, YIELD} state;
    co_t *coroutine;
    sem_t *finish;
};
typedef struct my_threads thr_t;

void mythtreads_init();
void mythreads_create(thr_t *thread, void (*func)(void *), void *arg);
void mythreads_join(thr_t *thread);
void mythread_destroy(thr_t *thread);
void mythread_yield();
void mythread_exit();
void find_thread(int thread_id, int *pos);

extern int thread_ids;
extern thr_t *threads_array;   // -1 terminated array of threads //

extern sem_t *lib_mtx; // we need to init //

void timer_init();

extern int finish;

double get_current_time();
void custom_sleep(double seconds);