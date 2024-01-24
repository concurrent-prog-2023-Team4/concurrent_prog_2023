//
#include <stdio.h>
#include <ucontext.h>
#include <stdlib.h>
#include <signal.h>

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




