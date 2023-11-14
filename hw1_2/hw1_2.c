// hw2 kai kala

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

int **workers; // initialized in main

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
    int *pos = (int *)varg;

    // workers[*pos][1] = 0;        // thread is available //
    while(1) 
    {
        while (workers[*pos][0] < 0) 
        {
            if (workers[*pos][3] == 1)    // terminate is 1, then TERMINATE //
            {
                workers[*pos][3] = -1;      // notify main that thread is closed //
                //printf("I am closing\n");
                pthread_exit(NULL);    
            }

        } //wait for work


        // /if(workers[*pos][0] >= 0)
        // {
        workers[*pos][1] = 1;    //  busy //
        workers[*pos][2] = find_prime(workers[*pos][0]);      // set flag //
        if(workers[*pos][2] == 0)
            printf("%d is a prime\n", workers[*pos][0]);
        else if (workers[*pos][2] == 1)
            printf("%d is not a prime\n", workers[*pos][0]);

        workers[*pos][4] = workers[*pos][4] + 1;        // add to array
        
        //printf("The result for number %d is %d for thread %lu\n",workers[*pos][0], workers[*pos][2], pthread_self());
        workers[*pos][2] = -1;
        workers[*pos][0] = -1;   // number is negative so invalid //
        workers[*pos][1] = 0;    // not busy //

            // if(workers[*pos][2] >= 0)     // flag is valid  //
            // {
                
                //workers[*pos][2] = -1;   // set flag invalid //

            // }

       // }
    }
}

int main(int argc, char *argv[])      // argument is N // 
{
    int num_threads = atoi(argv[1]);
    int number;
    // int pos;
    int i;
    int j;
    int *buffer = NULL;
    int buffer_pos = 0;
    int counter = 0;
    
    
    workers = (int**) calloc(num_threads, sizeof(int*));

    for(int i = 0; i < num_threads; i++)
    {
        workers[i] = (int*) calloc(5, sizeof(int));   
    }

    // open threads //
    pthread_t id[num_threads];
    int thread_ids[num_threads];

    for (int i = 0; i < num_threads; i++) 
    {
        thread_ids[i] = i;
    }

    for(j = 0; j < num_threads; j++)
    {
        workers[j][0] = -1; // invalid num
        workers[j][1] = 0;  // available
        workers[j][2] = -1; // invalid flag
        workers[j][3] = 0;  // not terminated
        workers[j][4] = 0;  // how many times find prime has been called
        pthread_create(&id[j], NULL, worker_thread, (void*) &thread_ids[j]);
    }
    
    i = 0;
    buffer = (int*) malloc(1*sizeof(int));
    do
    {
        scanf(" %d", &buffer[i]);
        i++;
        buffer = (int*) realloc(buffer, sizeof(int) * (i+1));
    }
    while (buffer[i-1] >= 0);

    while (buffer[buffer_pos] >= 0)
    {   
        for(i = 0; i < num_threads; i++)
        {
            if(workers[i][1] == 0)  // not busy //
            {
                if(workers[i][2] >= 0)  // flag is valid //
                {
                    // printf("The flag is %d\n", (workers[i][2]));
                    // workers[i][2] = -1;
                }
                //sleep(0.1);
                workers[i][0] = buffer[buffer_pos];
                buffer_pos++;

                if(buffer[buffer_pos] < 0)
                    break;
            }
        }
    } 

    // i = 0;
    // counter = 0;
    // while (1)
    // {
    //     if(i == num_threads)
    //         i = 0;
    //     if(workers[i][1] == 0)
    //         counter++;

    //     i++ ;
    // }

    while(1)
    {
        counter = 0;
        for(j = 0; j < num_threads; j++)
        {
            if(workers[j][1] == 0)     // thread available //
            {
                counter++;
            }
        }
        if(counter == num_threads)
            break;

    }
    printf("Counter is %d\n", counter);
    
    //int sum = 0;   
    for(i = 0; i < num_threads; i++)
    {
        //sum = sum + workers[i][4];
        workers[i][3] = 1;
    } 

    // for (int i = 0; i < num_threads; i++) {
    //     pthread_join(id[i], NULL);
    // }
    
    i = 0;
    counter = 0;
    while(counter < num_threads)
    {
        if(i == num_threads)
            i = 0;

        if(workers[i][3] == -1)
            counter++;
        i++;
    }

    printf("it is close\n");
    //sleep(10);

    int sum = 0;   
    for(i = 0; i < num_threads; i++)
    {
        sum = sum + workers[i][4];
    } 
    printf("The sum is %d\n", sum);

    return 0;
}








