#include "test.h"

// Shared data protected by the semaphore
int sharedData = 0;

// Custom semaphore declaration
sem_t *mySemaphore;

// Number of threads for testing
#define NUM_THREADS 4

// Function representing a critical section
void* criticalSection(void* arg) 
{
    int threadId = *((int*)arg);
    int curr_id = 0;
    while (1) 
    {
        // Wait on the custom semaphore before accessing sharedData
        if(sharedData >= 10000)
        {
            if(mySemaphore->value != 1)
                sem_up(mySemaphore);
            break;
        }
        sem_down(mySemaphore, threadId);
        // if (sem_down(mySemaphore, threadId) != 1) 
        // {
        //     fprintf(stderr, "Thread %d: Semaphore wait failed\n", threadId);
        //     exit(EXIT_FAILURE);
        // }

        // Critical section
        printf("Thread %d: Shared data before increment: %d\n", threadId, sharedData);
        sharedData++;
        printf("Thread %d: Shared data after increment: %d\n", threadId, sharedData);

        // Signal the custom semaphore to indicate completion of the critical section
        // if (sem_up(&mySemaphore) != 1) {
        //     fprintf(stderr, "Thread %d: Semaphore signal failed\n", threadId);
        //     exit(EXIT_FAILURE);
        // }
        sem_up(mySemaphore);

        // Sleep to simulate some work
        // sleep(1); // 100,000 microseconds = 0.1 seconds
    }
    printf("Thread %d terminated\n", threadId);

    find_thread(threadId, &curr_id);
    // mythread_destroy(&threads_array[curr_id]);
    // pthread_exit(NULL);

    sem_up(threads_array[curr_id].finish);

    while(1)
    {

    }

    return NULL;
}

int main() 
{
    // Create and initialize the custom semaphore
    int i = 0;
    mySemaphore = sem_create(mySemaphore, 1);
    int array[5] = {1, 2, 3, 4, 5};
    // if (mysem_create(&mySemaphore) != 0) 
    // {
    //     fprintf(stderr, "Semaphore creation failed\n");
    //     return EXIT_FAILURE;
    // }
    // mysem_init(&mySemaphore, 1);
    // mySemaphore.mutex = PTHREAD_MUTEX_INITIALIZER;
    // mySemaphore.cond = PTHREAD_COND_INITIALIZER;

    // Create an array to hold thread IDs
    // pthread_t threads[NUM_THREADS];
    // int threadIds[NUM_THREADS];

    mtx = NULL;
    mtx = sem_create(mtx, 1);

    threads_array = (thr_t *) malloc(5 * sizeof(thr_t));

    // Create multiple threads to test the custom semaphore
    mythreads_create(&threads_array[0], (void*) main, &i);
    // for (i = 1; i < NUM_THREADS; i++) 
    // {        
    //     mythreads_create(&threads_array[i], (void*) criticalSection, &array[i]);
    //     //printf("Thread create!\n");
    // }
    mythreads_create(&threads_array[1], (void*) criticalSection, &array[0]);
    mythreads_create(&threads_array[2], (void*) criticalSection, &array[1]);
    mythreads_create(&threads_array[3], (void*) criticalSection, &array[2]);
    // mythreads_create(&threads_array[0], (void*) criticalSection, &array[0]);
    // mythreads_create(&threads_array[0], (void*) criticalSection, &array[0]);

    threads_array[4].id = -1;

    mythtreads_init();

    while(1)
    {
        if(sharedData >= 10000)
        {
            break;
        }
    }

    mythreads_join(&threads_array[3]);
    mythreads_join(&threads_array[2]);
    mythreads_join(&threads_array[1]);

    mythread_destroy(&threads_array[3]);
    mythread_destroy(&threads_array[2]);
    mythread_destroy(&threads_array[1]);

    printf("Main exiting\n");
    // Wait for all threads to finish
    // for (int i = 0; i < NUM_THREADS; ++i) 
    // {
    //     //printf("waiting...\n");
    //     pthread_join(threads[i], NULL);
    // }

    // // Destroy the custom semaphore
    // if (mysem_destroy(&mySemaphore) != 1) {
    //     fprintf(stderr, "Semaphore destruction failed\n");
    //     return EXIT_FAILURE;
    // }

    return EXIT_SUCCESS;
}
