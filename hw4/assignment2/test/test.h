#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include "all.h"

void *thread_func(void *argv);
void *thread_func_2(void *argv);
int main();

sem_t *mtx;