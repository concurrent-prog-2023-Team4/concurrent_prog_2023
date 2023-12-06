// HW1 exercise 2
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include "../assignment1/mysem.h"

// #define DEBUG

struct worker   // a struct of info for each thread //
{
    int number;     // number to proccess //
    int status;     // can be -2, -1, 0, 1, it communicates with main //
    int size;       // size of numbers that each thread has proccessed //
    int *result[2];     // 2d array that keeps the value of each number and if it is prime or not //
    int pos;
    mysem_t sem;
    mysem_t finish;
    mysem_t term;
    // int done;
};

mysem_t mtx;

int done;

struct worker *workers;

int find_prime(int number)
{
    int i, flag = 0;

    // 0 and 1 are not prime numbers
    // change flag to 1 for non-prime number
    if (number == 0 || number == 1)
        flag = 1;

    for (i = 2; i <= number / 2; ++i) 
    {

        // if n is divisible by i, then n is not prime
        // change flag to 1 for non-prime number
        if (number % i == 0) 
        {
            flag = 1;
            break;
        }
    }
    
    return flag; 
}

void *worker_thread(void *varg)
{
    int result = -2;
    struct worker *ptr = (struct worker *)varg;
    int done_value;

    while(1)
    {
        mysem_down(&(ptr->sem));    // make semaphore down //
        pthread_yield();
    
        if((*ptr).status == -1)    // status is terminate //
        { 
            break;
        }

        #ifdef DEBUG
        printf("Thread %d processing number %d\n", (*ptr).pos, (*ptr).number);
        #endif
        (*ptr).result[0][(*ptr).size - 1] = (*ptr).number;
        (*ptr).result[1][(*ptr).size - 1] = find_prime((*ptr).number);
        (*ptr).size++;
        (*ptr).result[0] = (int *) realloc((*ptr).result[0], sizeof(int) * (*ptr).size);
        (*ptr).result[1] = (int *) realloc((*ptr).result[1], sizeof(int) * (*ptr).size);

        mysem_down(&mtx);
        done_value = done;
        result = mysem_up(&mtx);
        if(result == 0)
            printf("Semaphore mtx lost up\n");
        if(done_value == 1)
        {
            result = mysem_up(&(ptr->finish));
            if(result == 0)
                printf("semaphore finish lost up\n");
        }
        // result = mysem_up(&mtx);
        // if(result == 0)
        //     printf("Semaphore mtx lost up\n");

        (*ptr).status = 1;    // available //  
                

    }
    (*ptr).status = -2; 
    printf("Thread %d terminating...\n", (*ptr).pos);
    result = mysem_up(&(ptr->term));
    if(result == 0)
        printf("semaphore finish lost up\n");
    pthread_exit(NULL);
}

int main(int argc, char *argv[])
{
    int num_threads = 0;
    int i = 0, j = 0;
    int number = -1;
    int counter = 0;
    int sum = 0;
    done = 0;
    int result = -2;

    FILE *output_file;

    if(argv[1] != NULL)
        num_threads = atoi(argv[1]);
    else
    {
        printf("No argument\n");
        return -1;
    }

    pthread_t id[num_threads];      // ids of threads //

    workers = (struct worker *) calloc(num_threads, sizeof(struct worker));

    mysem_create(&mtx);
    mysem_init(&mtx, 1);

    for(i = 0; i < num_threads; i++)
    {
        workers[i].number = -1;
        workers[i].status = 1;
        workers[i].result[0] = (int*) calloc(1, sizeof(int));   // number //
        workers[i].result[1] = (int*) calloc(1, sizeof(int));   // result //
        workers[i].result[1][0] = 1;
        workers[i].result[0][0] = -1;
        workers[i].size = 1;
        workers[i].pos = i;
        // workers[i].done = 0;

        mysem_create(&(workers[i].sem));
        mysem_init(&(workers[i].sem), 0);   // init the semaphores //

        mysem_create(&(workers[i].finish));
        mysem_init(&(workers[i].finish), 0);   // init the semaphores //

        mysem_create(&(workers[i].term));
        mysem_init(&(workers[i].term), 0);
    }

    for(i = 0; i < num_threads; i++)
    {
        pthread_create(&id[i], NULL, worker_thread, (void*) (workers + i));
        printf("Thread %d created\n", i);
    }

    do
    {
        scanf("%d", &number);
        if(number <= 0)
        {
            printf("No more input\n");
            break;
        }
        while(1)
        {
            if(j == num_threads)
                j = 0;

            if(workers[j].status == 1)  // thread available //
            {
                break;
            }
            j++;
        }

        workers[j].number = number;     // give work //
        workers[j].status = 0;         // make thread busy //
        result = mysem_up(&(workers[j].sem));
        if(result == 0)
            printf("Semaphore sem lost up\n");

    }
    while(number > 0);

    mysem_down(&mtx);
    done = 1;
    result = mysem_up(&mtx);
    if(result == 0)
        printf("Semaphore mtx lost up");


    for(i = 0; i < num_threads; i++)
    {
        if(workers[i].status != 1)  // is available //
        {
            // mysem_down(&(workers[i].sem));
            mysem_down(&(workers[i].finish));
            pthread_yield();
        }
        workers[i].status = -1;
    }
    printf("finish check of available\n");

    for(i = 0; i < num_threads; i++)
    {
        //workers[i].status = -1;     // inform them to terminate //
        result = mysem_up(&(workers[i].sem));
        if(result == 0)
            printf("Semaphore sem lost up\n");
    }

    counter = 0;        // reset counter //
    printf("Main is waiting for threads to terminate!\n");

    for(i = 0; i < num_threads; i++)
    {
        if(workers[i].status == -2)
        {
            mysem_down(&(workers[i].term));
            pthread_yield();
        }
            // mysem_down(&workers[i].finish);
            
    }

    output_file = fopen("out.txt", "w");

    for(i = 0; i < num_threads; i++)
    {
        for(j = 0; j < workers[i].size - 1; j++)
        {
            if( workers[i].result[1][j] == 1)
            {
                sum++;
                fprintf(output_file, "The number %d is NOT PRIME!\n", workers[i].result[0][j]);
            }
            else if( workers[i].result[1][j] == 0)
            {
                sum++;
                fprintf(output_file, "The number %d is PRIME!\n", workers[i].result[0][j]);
            }
        }
    }

    printf("Total numbers that program calulated are %d\n\n", sum); 

    printf("Main exiting...\n");

    fclose(output_file); 

    return 0;       // EXIT //
}

