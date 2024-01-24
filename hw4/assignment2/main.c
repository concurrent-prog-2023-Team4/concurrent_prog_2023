#include "my_threads.h"

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
    int i = 0;
    // Initialize the main thread
    mythtreads_init();

    // Create the first thread
    mythreads_create(&threads_array[0], thread_func, (void *) i);

    // Create the second thread
    mythreads_create(&threads_array[1], thread_func, (void*) (i + 1) );

    // Create the third thread
    mythreads_create(&threads_array[2], thread_func, (void*) (i + 2) );

    // Join the first thread
    mythreads_join(&threads_array[0]);

    // Join the second thread
    mythreads_join(&threads_array[1]);

    // Join the third thread
    mythreads_join(&threads_array[2]);

    return 0;
}