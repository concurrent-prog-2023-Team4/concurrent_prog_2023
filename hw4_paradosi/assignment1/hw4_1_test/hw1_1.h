#include <stdio.h>
#include <pthread.h>
#include "hw1_1_lib.h"
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "../coroutines.h"

#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_RESET   "\x1b[0m"
#define ANSI_COLOR_BLUE    "\x1b[34m"

co_t *reader_cot, *writer_cot, *main_co;

char *input;

void *writer(void *argv);
void *reader(void *argv);

