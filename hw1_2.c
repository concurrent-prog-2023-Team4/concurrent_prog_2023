// hw2 kai kala

#include <stdio.h>
#include <stdlib.h>
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
    
    // flag is 0 for prime numbers
    // if (flag == 0)
    //     printf("%d is a prime number.", number);
    // else
    //     printf("%d is not a prime number.", number);
    // printf("The result is %d\n", flag);
    return flag; //
}

void *worker_thread(void *varg)
{
    int *pos = (int *)varg;

    // workers[*pos][1] = 0;        // thread is available //
    while(1) 
    {
        if (workers[*pos][3])    // terminate is 1, then TERMINATE //
            pthread_exit(NULL);    
        while (workers[*pos][0] < 0); //wait for work


        // /if(workers[*pos][0] >= 0)
        // {
        workers[*pos][1] = 1;    //  busy //
        workers[*pos][2] = find_prime(workers[*pos][0]);      // set flag //

        printf("The flag is %d\n", (workers[*pos][2]));
                    workers[*pos][2] = -1;
            // printf("The result is %d\n", workers[*pos][2]);
        workers[*pos][0] = -1;   // number is negative so invalid //
        workers[*pos][1] = 0;    // not busy //

            // if(workers[*pos][2] >= 0)     // flag is valid  //
            // {
                
                //workers[*pos][2] = -1;   // set flag invalid //

            // }

       // }
    }
    return NULL;
}



int main(int argc, char *argv[])      // argument is N // 
{
    int num_threads = atoi(argv[1]);
    int number;
    // int pos;
    int i;
    int j;
    
    
    workers = (int**) calloc(num_threads, sizeof(int*));

    for(int i = 0; i < num_threads; i++)
    {
        workers[i] = (int*) calloc(4, sizeof(int));     // pos 0: num, pos 1: busy, pos 2: flag, pos 3: terminate
    }

 

    // open threads 
    pthread_t id[num_threads];
    int thread_ids[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

    for(j = 0; j < num_threads; j++)
    {
        pthread_create(&id[j], NULL, worker_thread, (void*) &thread_ids[j]);
        workers[j][0] = -1; // invalid num
        workers[j][1] = 0;  // available
        workers[j][2] = -1; // invalid flag
        workers[j][3] = 0;  // not terminated
    }

    do
    {   
        
        // if(workers[0][1] == 0)      // it is available //
        // {
        //     printf("Enter a positive integer: ");
        //     scanf("%d", &number);
        //     workers[0][0] = number;
        //     while(workers[0][2] < 0);   // it is valid //
        //     printf("The result is %d\n", workers[0][2]);
        // }

        for(i = 0; i < num_threads; i++)
        {
            if(workers[i][1] == 0)  // not busy //
            {
                if(workers[i][2] >= 0)  // flag is valid //
                {
                    // printf("The flag is %d\n", (workers[i][2]));
                    // workers[i][2] = -1;
                }
                
                scanf("%d", &number);

                if(number < 0)
                {   
                    return 0;
                    // for(j = 0; j < num_threads; j++)
                    // {
                    //     workers[j][3] = 1;
                    // }
                }
                workers[i][0] = number;

            }
        }


        // for() 
        // {
        //     if(not_busy)
        //         if(flag >= 0)
        //             print flag
        //             flag = negative
                    
        //         scanf
        // }
    } while (number >= 0);
    
    // for (j = 0; j < num_threads; j++) {
    //     pthread_exit(id[j]); // Cancel each thread
    // }


  return 0;
}