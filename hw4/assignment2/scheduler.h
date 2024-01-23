//
#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include "my_threads.h"

void handle_alarm(int signum);
void set_alarm(int seconds);

int current_thread;     // index of current thread in threads_array //
void *thread_func(void *arg);