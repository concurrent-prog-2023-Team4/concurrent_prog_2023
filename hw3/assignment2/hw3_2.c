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

    // mysem_up(&(ptr->start));    // notify main that thread created //
    while(1)
    {
        // mysem_down(&(ptr->sem));    // make semaphore down //
        // pthread_yield();


        enterMonitor(give_work);
        available_threads++;
        wait(give_work);
        if((*ptr).status == -1)    // status is terminate //
        { 
            break;
        }
        if(global_number != -1)
        {
            (*ptr).number = global_number;
            global_number = -1;
            total_num++;
            available_threads--;
        }
        else
        {
            available_threads--;
            exitMonitor(give_work);
            continue;
        }
        
        // available_threads--;
        exitMonitor(give_work);
    


        enterMonitor(get_number);
        did_notify = 1;
        signal(get_number);
        
        printf("Thread %d wakes up main\n", (*ptr).pos);
        exitMonitor(get_number);

        #ifdef DEBUG
        printf("Thread %d processing number %d\n", (*ptr).pos, (*ptr).number);
        #endif
        (*ptr).result[0][(*ptr).size - 1] = (*ptr).number;
        (*ptr).result[1][(*ptr).size - 1] = find_prime((*ptr).number);
        (*ptr).size++;
        (*ptr).result[0] = (int *) realloc((*ptr).result[0], sizeof(int) * (*ptr).size);
        (*ptr).result[1] = (int *) realloc((*ptr).result[1], sizeof(int) * (*ptr).size);

        enterMonitor(main_monitor);
        signal(main_monitor);
        exitMonitor(main_monitor);
    }
    (*ptr).status = -2; 
    printf("Thread %d terminating...\n", (*ptr).pos);

    enterMonitor(main_monitor);
    signal(main_monitor);
    exitMonitor(main_monitor);

    pthread_exit(NULL);
}

int main(int argc, char *argv[])
{
    int num_threads = 0;
    int i = 0, j = 0;
    int number = -1;
    int sum = 0;
    int result = -2;
    int available;

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

    // mysem_create(&mtx);
    // mysem_init(&mtx, 1);

    // mysem_create(&sem_main);
    // mysem_init(&sem_main, 0);

    // blocked_main = 0;
    // curr_thread = -1;

    did_notify = 0;

    get_number = initMonitor(get_number);
    give_work = initMonitor(give_work);
    main_monitor = initMonitor(main_monitor);

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

        // mysem_create(&(workers[i].sem));
        // mysem_init(&(workers[i].sem), 0);   // init the semaphores //

        // mysem_create(&(workers[i].finish));
        // mysem_init(&(workers[i].finish), 0);   // init the semaphores //

        // mysem_create(&(workers[i].term));
        // mysem_init(&(workers[i].term), 0);
        
        // mysem_create(&(workers[i].start));
        // mysem_init(&(workers[i].start), 0);
    }

    for(i = 0; i < num_threads; i++)
    {
        pthread_create(&id[i], NULL, worker_thread, (void*) (workers + i));
        // mysem_down(&(workers[i].start));
        // prepei na perimenei na ftiaxtoun 
        printf("Thread %d created\n", i);
    }
    total_num = 0;
    do
    {
        scanf("%d", &number);
        if(number <= 0)
        {
            printf("No more input\n");
            break;
        }
        enterMonitor(give_work);
        global_number = number;
        exitMonitor(give_work);

        enterMonitor(get_number);
        did_notify = 0;
        exitMonitor(get_number);

        // for(i = 0; i < num_threads; i++)
        // {
        //     if(workers[i].status == 1)
        //     {
        //         curr_thread = i;
        //         printf("find available\n");
        //         break;
        //     }
        // }
        // if(i == num_threads)    // all are busy //
        // {
        //     printf("before mtx\n");
        //     mysem_down(&mtx);
        //     blocked_main = 1;
        //     mysem_up(&mtx);
        //     printf("Main is going down for real\n");
        //     mysem_down(&sem_main);
        // }

        enterMonitor(give_work);
        available = available_threads;
        exitMonitor(give_work);
        
        enterMonitor(main_monitor);
        if(available_threads == 0)
        {
            printf("Main sleeps cause all threads busy\n");
            wait(main_monitor);
        }
        exitMonitor(main_monitor);

        enterMonitor(give_work);
        available = available_threads;
        if(available > 0)
        {
            signal_all(give_work);
            exitMonitor(give_work);

            enterMonitor(get_number);
            while(did_notify != 1)
            {
                printf("Main: sleep now and available is %d and total num is %d\n", available_threads, total_num);
                wait(get_number);
            }
            exitMonitor(get_number);
        }     
        // workers[curr_thread].number = number;     // give work //
        // workers[curr_thread].status = 0;         // make thread busy //
        // result = mysem_up(&(workers[curr_thread].sem));
        // if(result == 0)
        //     printf("Semaphore sem lost up\n");
    }
    while(number > 0);

    #ifdef DEBUG
    // printf("before mutex and value is %d\n", semctl(mtx.sem_id, 0, GETVAL));
    #endif 

    // sleep(2);

    enterMonitor(give_work);
    available = available_threads;
    exitMonitor(give_work);

    while(available != num_threads)
    {
        enterMonitor(main_monitor);
        wait(main_monitor);
        exitMonitor(main_monitor);

        enterMonitor(give_work);
        available = available_threads;
        exitMonitor(give_work);

        printf("Available %d\n", available_threads);
    }   // when you finish this, all workers are available //

    for(i = 0; i < num_threads; i++)
    {
        workers[i].status = -1;
    }
    enterMonitor(give_work);
    signal_all(give_work);
    exitMonitor(give_work);


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
     
    // if(mysem_destroy(&mtx))
    //     printf("Semaphore mtx destroyed succesfully!\n");
    // for (i = 0; i < num_threads; i++) 
    // {
    //     if (mysem_destroy(&(workers[i].sem)))
    //         printf("Semaphore workers[%d].sem destroyed succesfully!\n", i);
    //     if (mysem_destroy(&(workers[i].finish)))
    //         printf("Semaphore workers[%d].finish destroyed succesfully!\n", i);
    //     if (mysem_destroy(&(workers[i].term)))
    //         printf("Semaphore workers[%d].term destroyed succesfully!\n", i);
    //     if (mysem_destroy(&(workers[i].start)))
    //         printf("Semaphore workers[%d].start destroyed succesfully!\n", i);
    // }
   
    destroyMonitor(main_monitor);
    destroyMonitor(get_number);
    destroyMonitor(give_work);

    printf("Main exiting...\n");

    fclose(output_file); 

    return 0;       // EXIT //
}

