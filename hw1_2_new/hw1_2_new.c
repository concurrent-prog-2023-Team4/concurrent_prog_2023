// thank you maria!
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

int **workers;

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
    int *ptr = (int *)varg;
    int result = -1;

    while(1)
    {
        while(1)
        {
            if( (ptr[1] == -1) || (ptr[1] == 0) )    // status is terminate //
            {
                break;  // terminate //
            }
        }
        if(ptr[1] == -1)    // status is terminate //
        {
            break;  // terminate //
        }

        if(ptr[1] == 0)    // is busy //
        {
            result = find_prime(ptr[0]);
            ptr[2] = ptr[2] + 1;        // add iterations //
            ptr[1] = 1;    // available //
        }
    }
    ptr[1] = -2;   // notify main i am terminating //
    pthread_exit(NULL);
}

int main(int argc, char *argv[])
{
    int num_threads;
    int i = 0, j = 0;
    int number = -1;
    int flag;
    int counter = 0;

    if(argv[1] != NULL)
        num_threads = atoi(argv[1]);

    pthread_t id[num_threads];      // ids of threads //


    workers = (int**) calloc(num_threads, sizeof(int*));

    for(int i = 0; i < num_threads; i++)        // create array of workers
    {
        workers[i] = (int*) calloc(3, sizeof(int));   
    }

    for(i = 0; i < num_threads; i++)
    {
        workers[i][0] = -1;     // initialize number //
        workers[i][1] = 1;      // status available //
        workers[i][2] = 0;     // number of iterations // 
    }

    for(i = 0; i < num_threads; i++)
    {
        pthread_create(&id[i], NULL, worker_thread, (void*) workers[i]);
    }
    printf("Threads created\n");

    do
    {
        //printf("Add input\n");
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

            if(workers[j][1] == 1)
            {
                break;
            }
            j++;
        }

        workers[j][0] = number;     // give work to slaves //
        workers[j][1] = 0;         // make thread busy //


    }while(number > 0);

    while(1)
    {
        counter = 0;
        for(j = 0; j < num_threads; j++)
        {
            if(workers[j][1] == 1)     // thread available //
            {
                counter++;
            }
        }
        if(counter == num_threads)
            break;

    }
    printf("Counter is %d\n", counter);

    for(i = 0; i < num_threads; i++)
    {
        workers[i][1] = -1;     // inform them to terminate //
    }

    counter = 0;        // reset counter //
    while(1)
    {
        if(counter == num_threads)
            break;

        if(j == num_threads)
            j = 0;

        if(workers[j][1] == -2)     // thread terminated //
        {
            counter++;
        }
        j++;
    }

    int sum = 0;
    for(i = 0; i < num_threads; i++)
    {
        sum = sum + workers[i][2];
    }
    printf("Exit success, sum is %d!\n", sum);

    return 0;       // EXIT
}

