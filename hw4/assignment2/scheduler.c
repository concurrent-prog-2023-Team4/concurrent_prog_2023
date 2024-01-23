//
#include "scheduler.h"

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
            threads_array[current_thread].coroutine->next_cot = threads_array[i].coroutine;
            mycoroutines_switchto(&threads_array[current_thread].coroutine);
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



