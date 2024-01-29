// #include "all.h"
#include "mysem.h"
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define DEBUG
#define ANSI_COLOR_RED     "\x1b[31m"   // define color codes to print TCL messages //
#define ANSI_COLOR_RESET   "\x1b[0m"
#define ANSI_COLOR_BLUE    "\x1b[34m"

mysem_t mtx;
mysem_t read_sem;
mysem_t write_sem;

int readers_waiting;
int writers_waiting;
int reading;
int writing;

int reader_num;
int writer_num;