#include <stdio.h>
#include <pthread.h>
#include "../assignment1/mysem.h"
#include <stdlib.h>
#include <unistd.h>

#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_RESET   "\x1b[0m"
#define ANSI_COLOR_BLUE    "\x1b[34m"

struct train
{   
    int passengers_waiting;
    int max_passengers;
    int on_train;
    pthread_t *passengers;
    int exit;
};
typedef struct train train_t;

train_t *train;

mysem_t mtx;    // to read struct //
mysem_t train_sem;
mysem_t pass_wait; // blocks waiting passengers //
mysem_t pass_ride; // blocks passengers on train //

void *train_function();
void *passengers_function(void *varg);