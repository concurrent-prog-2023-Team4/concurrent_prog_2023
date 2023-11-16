// HW1 exercise 2
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#define DEBUG    

struct worker
{
    int number;
    int status;
    int size;
    int *result[2];
};

struct worker *workers;
// int **workers;

int **results;

int find_prime(int number)
{
    int i, flag = 0;

    // 0 and 1 are not prime numbers
    // change flag to 1 for non-prime number
    if (number == 0 || number == 1)
        flag = 1;

    for (i = 2; i <= number / 2; ++i) {

        // if n is divisible by i, then n is not prime
        // change flag to 1 for non-prime number
        if (number % i == 0) {
        flag = 1;
        break;
        }
    }
    
    return flag; 
}

void *worker_thread(void *varg)
{
    struct worker *ptr = (struct worker *)varg;

    while(1)
    {
        while(1)
        {
            if( ((*ptr).status == -1) || ((*ptr).status == 0) )    // status is terminate //
            {
                break;  // terminate //
            }
        }
        if((*ptr).status == -1)    // status is terminate //
        {
            break;  // terminate //
        }

        if((*ptr).status == 0)    // is busy //
        {
            (*ptr).result[0][(*ptr).size - 1] = (*ptr).number;
            (*ptr).result[1][(*ptr).size - 1] = find_prime((*ptr).number);
            (*ptr).size++;
            (*ptr).result[0] = (int *) realloc((*ptr).result[0], sizeof(int) * (*ptr).size);
            (*ptr).result[1] = (int *) realloc((*ptr).result[1], sizeof(int) * (*ptr).size);
            // ptr[2] = ptr[2] + 1;        // add iterations //
            (*ptr).status = 1;    // available //
        }
    }
    (*ptr).status = -2;   // notify main i am terminating //
    pthread_exit(NULL);
}

int main(int argc, char *argv[])
{
    int num_threads;
    int i = 0, j = 0;
    int number = -1;
    int counter = 0;
    int sum = 0;


    if(argv[1] != NULL)
        num_threads = atoi(argv[1]);

    pthread_t id[num_threads];      // ids of threads //

    workers = (struct worker *) calloc(num_threads, sizeof(struct worker));

    for(i = 0; i < num_threads; i++)
    {
        workers[i].number = -1;
        workers[i].status = 1;
        workers[i].result[0] = (int*) calloc(1, sizeof(int));   // number
        workers[i].result[1] = (int*) calloc(1, sizeof(int));   // result
        workers[i].result[1][0] = 1;
        workers[i].result[0][0] = -1;
        workers[i].size = 1;
    }

    for(i = 0; i < num_threads; i++)
    {
        pthread_create(&id[i], NULL, worker_thread, (void*) (workers + i) );
    }
    printf("Threads created\n");

    do
    {
        scanf("%d", &number);
        if(number <= 0)
        {
            printf("Invalid number\n");
            break;
        }
        while(1)
        {
            if(j == num_threads)
                j = 0;

            if(workers[j].status == 1)
            {
                break;
            }
            j++;
        }

        workers[j].number = number;     // give work to slaves //
        workers[j].status = 0;         // make thread busy //


    }while(number > 0);

    while(1)
    {
        counter = 0;
        for(j = 0; j < num_threads; j++)
        {
            if(workers[j].status == 1)     // thread available //
            {
                counter++;
            }
        }
        if(counter == num_threads)
            break;

    }

    for(i = 0; i < num_threads; i++)
    {
        workers[i].status = -1;     // inform them to terminate //
    }

    counter = 0;        // reset counter //
    while(1)
    {
        if(counter == num_threads)
            break;

        if(j == num_threads)
            j = 0;

        if(workers[j].status == -2)     // thread terminated //
        {
            counter++;
        }
        j++;
    }

    for(i = 0; i < num_threads; i++)
    {
        for(j = 0; j < workers[i].size - 1; j++)
        {
            if( workers[i].result[1][j] == 1)
            {
                sum++;
                printf("The number %d is NOT PRIME!\n", workers[i].result[0][j]);
            }
            else if( workers[i].result[1][j] == 0)
            {
                sum++;
                printf("The number %d is PRIME!\n", workers[i].result[0][j]);
            }
        }
    }

    #ifdef DEBUG
    printf("Total numbers are %d\n", sum);  
    #endif

    return 0;       // EXIT
}

