#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include "../monitor_lib/monitors.h"
#define DEBUG
struct worker   // a struct of info for each thread //
{
    int number; // number to proccess //
    int status; // can be -2, -1, 0, 1, it communicates with main //
    int size; // size of numbers that each thread has proccessed //
    int *result[2]; // 2d array that keeps the value of each number and if it is prime or not //
    int pos; // indicates the number of worker //
    int did_notify;
    Monitor *give_work;
    Monitor *finish_work;
};
struct worker *workers;
Monitor *main_monitor, *main_finish;

int global_number;
int curr_thread;
int available_threads;

int total_num;
int num_threads;

int find_prime(int number);
void *worker_thread(void *varg);
