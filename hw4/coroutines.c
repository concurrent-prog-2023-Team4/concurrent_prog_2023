#include "coroutines.h"

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

int mycoroutines_create(co_t *thread, void (*routine)(void *), void *arg)
{
    // Initialize the thread coroutine
    getcontext(thread->cot);
    (*thread).cot->uc_link = 0;
    (*thread).cot->uc_stack.ss_sp = (*thread).stack;
    (*thread).cot->uc_stack.ss_size = STACK_SIZE;
    (*thread).cot->uc_stack.ss_flags = 0;
    makecontext(thread->cot, (void (*)(void))routine, 1, arg);

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
