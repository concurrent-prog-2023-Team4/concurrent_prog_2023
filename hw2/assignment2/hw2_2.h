#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include "../assignment1/mysem.h"

// #define DEBUG

struct worker   // a struct of info for each thread //
{
    int number; // number to proccess //
    int status; // can be -2, -1, 0, 1, it communicates with main //
    int size; // size of numbers that each thread has proccessed //
    int *result[2]; // 2d array that keeps the value of each number and if it is prime or not //
    int pos; // indicates the number of worker //
    mysem_t sem;
    mysem_t finish;
    mysem_t term;
};

mysem_t mtx;
struct worker *workers;

int done;

int find_prime(int number);
void *worker_thread(void *varg);
