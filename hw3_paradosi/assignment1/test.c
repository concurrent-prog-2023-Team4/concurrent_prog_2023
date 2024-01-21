#include "test.h"

// Shared data protected by the semaphore
int sharedData = 0;

// Custom semaphore declaration
mysem_t mySemaphore;

// Number of threads for testing
#define NUM_THREADS 4

// Function representing a critical section
void* criticalSection(void* arg) 
{
    int threadId = *((int*)arg);
    while (1) 
    {
        // Wait on the custom semaphore before accessing sharedData
        if (mysem_down(&mySemaphore) != 1) 
        {
            fprintf(stderr, "Thread %d: Semaphore wait failed\n", threadId);
            exit(EXIT_FAILURE);
        }

        // Critical section
        printf("Thread %d: Shared data before increment: %d\n", threadId, sharedData);
        sharedData++;
        printf("Thread %d: Shared data after increment: %d\n", threadId, sharedData);

        // Signal the custom semaphore to indicate completion of the critical section
        if (mysem_up(&mySemaphore) != 1) {
            fprintf(stderr, "Thread %d: Semaphore signal failed\n", threadId);
            exit(EXIT_FAILURE);
        }

        // Sleep to simulate some work
        sleep(1); // 100,000 microseconds = 0.1 seconds
    }

    pthread_exit(NULL);

    return NULL;
}

int main() {
    // Create and initialize the custom semaphore
    if (mysem_create(&mySemaphore) != 0) 
    {
        fprintf(stderr, "Semaphore creation failed\n");
        return EXIT_FAILURE;
    }
    mysem_init(&mySemaphore, 1);
    // mySemaphore.mutex = PTHREAD_MUTEX_INITIALIZER;
    // mySemaphore.cond = PTHREAD_COND_INITIALIZER;

    // Create an array to hold thread IDs
    pthread_t threads[NUM_THREADS];
    int threadIds[NUM_THREADS];

    // Create multiple threads to test the custom semaphore
    for (int i = 0; i < NUM_THREADS; ++i) 
    {
        threadIds[i] = i + 1;
        if (pthread_create(&threads[i], NULL, criticalSection, &threadIds[i]) != 0) {
            fprintf(stderr, "Thread creation failed\n");
            return EXIT_FAILURE;
        }
        //printf("Thread create!\n");
    }

    // Wait for all threads to finish
    for (int i = 0; i < NUM_THREADS; ++i) 
    {
        //printf("waiting...\n");
        pthread_join(threads[i], NULL);
    }

    // Destroy the custom semaphore
    if (mysem_destroy(&mySemaphore) != 1) {
        fprintf(stderr, "Semaphore destruction failed\n");
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
