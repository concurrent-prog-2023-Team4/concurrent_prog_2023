#include "../assignment2/mythread_lib.h"
#include <stdio.h>
#include <stdlib.h>

#define DEBUG
#define ANSI_COLOR_RED     "\x1b[31m"   // define color codes to print TCL messages //
#define ANSI_COLOR_RESET   "\x1b[0m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_GREEN   "\x1b[32m"

sem_t *mtx;
sem_t *read_sem;
sem_t *write_sem;

int readers_waiting;
int writers_waiting;
int reading;
int writing;

int reader_num;
int writer_num;