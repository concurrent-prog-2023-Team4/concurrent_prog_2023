//
#include <stdio.h>
#include <ucontext.h>
#include <stdlib.h>
#include <signal.h>

#define STACK_SIZE 16384

struct coroutine
{
    ucontext_t *cot;
    char stack[16384];
};
typedef struct coroutine co_t;

int mycoroutines_init(co_t *coroutines);
int mycoroutines_create(co_t *coroutines, void (*func)(void *), void *arg);
int mycoroutins_switchto(co_t *coroutines, co_t *coroutines2);
int mycoroutines_destroy(co_t *coroutines);




