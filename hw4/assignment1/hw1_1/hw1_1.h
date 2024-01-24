#include <stdio.h>
#include <pthread.h>
#include "hw1_1_lib.h"
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "../coroutines.h"

co_t *thread_1, *thread_2, *main_co;

char *input;

void *thread_func_2(void *argv);
void *thread_func_1(void *argv);

