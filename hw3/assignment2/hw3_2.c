// HW1 exercise 2
#include "hw3_2.h"

int find_prime(int number)
{
    int i, flag = 0;
    if (number == 0 || number == 1)
        flag = 1;

    for (i = 2; i <= number / 2; ++i) 
    {
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
    int available;
    int status;

    // mysem_up(&(ptr->start));    // notify main that thread created //
    while(1)
    {
        // enterMonitor(main_monitor);
        // available = available_threads;
        // exitMonitor(main_monitor);

        // enterMonitor(main_finish);
        // if(available == num_threads)
        // {
        //     signal(main_finish);
        // }
        // exitMonitor(main_finish);

        enterMonitor((*ptr).give_work);
        // available_threads++;
        if((*ptr).status == 1)
            wait((*ptr).give_work);
        if((*ptr).status == -1)    // status is terminate //
        { 
            exitMonitor((*ptr).give_work);
            break;
        }
        exitMonitor((*ptr).give_work);

        #ifdef DEBUG
        printf("Thread %d processing number %d\n", (*ptr).pos, (*ptr).number);
        #endif
        (*ptr).result[0][(*ptr).size - 1] = (*ptr).number;
        (*ptr).result[1][(*ptr).size - 1] = find_prime((*ptr).number);
        (*ptr).size++;
        (*ptr).result[0] = (int *) realloc((*ptr).result[0], sizeof(int) * (*ptr).size);
        (*ptr).result[1] = (int *) realloc((*ptr).result[1], sizeof(int) * (*ptr).size);

        (*ptr).status = 1;
        enterMonitor(main_monitor);
        available_threads++;
        curr_thread = (*ptr).pos;
        // ((*ptr).number = -1);
        printf("Thread %d notify main\n", (*ptr).pos);
        signal(main_monitor);
        exitMonitor(main_monitor);

        enterMonitor((*ptr).give_work);
        status = (*ptr).status;
        exitMonitor((*ptr).give_work);  
        if (status != 0)
        {
            enterMonitor((*ptr).finish_work);
            // (*ptr).did_notify = 1;
            printf("Thread %d wakes up main to finish\n", (*ptr).pos);
            signal((*ptr).finish_work);
            exitMonitor((*ptr).finish_work);
        }

    }
    (*ptr).status = -2; 
    printf("Thread %d terminating...\n", (*ptr).pos);

    pthread_exit(NULL);
}

int main(int argc, char *argv[])
{
    num_threads = 0;
    int i = 0, j = 0;
    int number = -1;
    int sum = 0;
    int result = -2;
    int available;
    int curr_status = -3;
    int curr_thread_temp = -1;

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

    available_threads = num_threads;
    main_monitor = (Monitor *) malloc(2*sizeof(Monitor));
    main_finish = (Monitor *) malloc(2*sizeof(Monitor));
    main_monitor = initMonitor(main_monitor);
    main_finish = initMonitor(main_finish);

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
        workers[i].did_notify = 0;

        workers[i].give_work = (Monitor *) malloc(2*sizeof(Monitor));
        workers[i].finish_work = (Monitor *) malloc(2*sizeof(Monitor));
        
        workers[i].give_work = initMonitor(workers[i].give_work);
        workers[i].finish_work = initMonitor(workers[i].finish_work);
    }

    for(i = 0; i < num_threads; i++)
    {
        pthread_create(&id[i], NULL, worker_thread, (void*) (workers + i));
        // prepei na perimenei na ftiaxtoun 
        printf("Thread %d created\n", i);
    }
    usleep(10);
    total_num = 0;
    do
    {
        scanf("%d", &number);
        if(number <= 0)
        {
            printf("No more input\n");
            break;
        }

        for(i = 0; i < num_threads; i++)
        {
            enterMonitor(workers[i].give_work);
            curr_status = workers[i].status;
            exitMonitor(workers[i].give_work);
            if(curr_status == 1)
            {
                enterMonitor(main_monitor);
                curr_thread = i;
                exitMonitor(main_monitor);
                break;  // found available //
            }
        }
        if(i == num_threads)
        {
            enterMonitor(main_monitor);
            printf("Main is going for sleep\n");
            wait(main_monitor);
            exitMonitor(main_monitor);
            printf("Main wakes up from thread %d\n", curr_thread);
        }
        enterMonitor(main_monitor);
        curr_thread_temp = curr_thread;
        available_threads--;
        exitMonitor(main_monitor);

        enterMonitor(workers[curr_thread_temp].give_work);
        workers[curr_thread_temp].number = number;
        workers[curr_thread_temp].status = 0;
        signal(workers[curr_thread_temp].give_work);
        exitMonitor(workers[curr_thread_temp].give_work);
    }
    while(number > 0);

    #ifdef DEBUG
    // printf("before mutex and value is %d\n", semctl(mtx.sem_id, 0, GETVAL));
    #endif 
        
    for(i = 0; i < num_threads; i++)
    {
        enterMonitor(workers[i].finish_work);
        if(workers[i].status == 0)
        {
            // if(workers[i].did_notify != 1)
                wait(workers[i].finish_work);
                printf("Main waits for thread to finish\n");
        }
        workers[i].did_notify = 0;
        exitMonitor(workers[i].finish_work);
    }

    for(i = 0; i < num_threads; i++)
    {
        enterMonitor(workers[i].give_work);
        workers[i].status = -1;
        exitMonitor(workers[i].give_work);
    }

    for(i = 0; i < num_threads; i++)
    {
        enterMonitor(workers[i].give_work);
        signal(workers[i].give_work);
        exitMonitor(workers[i].give_work);
    }

    // for(i = 0; i < num_threads; i++)
    // {
    //     enterMonitor(workers[i].finish_work);
    //     if(workers[i].status != -2)
    //     {
    //         // if(workers[i].did_notify != 1)
    //             wait(workers[i].finish_work);
    //     }
    //     exitMonitor(workers[i].finish_work);
    // }
    // for(i = 0; i < num_threads; i++)
    // {
    //     if(workers[i].status == 0)
    //     {
    //         enterMonitor(workers[i].give_work);
    //         workers[i].status = -1;
    //         signal(workers[i].give_work);
    //         exitMonitor(workers[i].give_work);

    //         enterMonitor(workers[i].finish_work);
    //         wait(workers[i].finish_work);
    //         exitMonitor(workers[i].finish_work);
    //     }
        

    //     enterMonitor(workers[i].finish_work);
    //     if(workers[i].status != -2)
    //     {
    //         wait(workers[i].finish_work);
    //     }
    //     exitMonitor(workers[i].finish_work);
    // }

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
   
    // destroyMonitor(main_monitor);
    // destroyMonitor(main_finish);
    // for(i = 0; i < num_threads; i++)
    // {
    //     destroyMonitor(workers[i].give_work);
    // }
    // printf("Monitors succesfully destroyed!\n");

    printf("Main exiting...\n");

    fclose(output_file); 

    return 0;       // EXIT //
}

