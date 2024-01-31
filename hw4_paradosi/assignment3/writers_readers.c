#include "writers_readers.h"

void *writer()
{
    int curr_id;

    sem_down(mtx);
    curr_id = writer_num;
    writer_num++;
    sem_up(mtx);

    sem_down(mtx);
    if (reading + writing > 0) 
    {
        writers_waiting++;
        sem_up(mtx);
        sem_down(write_sem);
    }
    else 
    {
        writing++;
        sem_up(mtx);;
    }

    /* write */
    printf(ANSI_COLOR_RED "Writer %d enters critical section\nINFO: writing: %d reading: %d   readers_waiting: %d writers_waiting: %d\n\n" ANSI_COLOR_RESET, curr_id, writing, reading, readers_waiting, writers_waiting);
    custom_sleep(2);

    sem_down(mtx);
    writing--;
    if (readers_waiting > 0) 
    {
        readers_waiting--;
        reading++; 
        sem_up(read_sem);
        printf(ANSI_COLOR_RED "Writer %d enters critical section\nINFO: writing: %d reading: %d   readers_waiting: %d writers_waiting: %d\n\n" ANSI_COLOR_RESET, curr_id, writing, reading, readers_waiting, writers_waiting);
    }
    else 
    {
        if (writers_waiting > 0) 
        {
            writers_waiting--;
            writing++;
            sem_up(write_sem);
        }
        printf(ANSI_COLOR_RED "Writer %d enters critical section\nINFO: writing: %d reading: %d   readers_waiting: %d writers_waiting: %d\n\n" ANSI_COLOR_RESET, curr_id, writing, reading, readers_waiting, writers_waiting);
        sem_up(mtx);
    }

    mythread_exit();

    return NULL;
}

void *reader()
{
    int curr_id;

    sem_down(mtx);
    curr_id = reader_num;
    reader_num++;
    sem_up(mtx);;

    sem_down(mtx);
    if (writing + writers_waiting > 0) 
    {
        readers_waiting++;
        sem_up(mtx);;
        sem_down(read_sem);
        if (readers_waiting > 0) 
        {
            readers_waiting--;
            reading++;
            sem_up(read_sem);
        }
        else 
        {
            sem_up(mtx);
        }
    }
    else 
    {
        reading++;
        sem_up(mtx);
    }

    /* read */
    printf(ANSI_COLOR_BLUE "Reader %d enters critical section\nINFO: writing: %d reading: %d  readers_waiting: %d writers_waiting: %d\n\n" ANSI_COLOR_RESET, curr_id, writing, reading, readers_waiting, writers_waiting);
    custom_sleep(2);
    // mythread_yield();

    sem_down(mtx);
    reading--;
    if ( (reading == 0) && (writers_waiting > 0) )
    {
        writers_waiting--;
        writing++;
        sem_up(write_sem);
    }
    printf(ANSI_COLOR_BLUE "Reader %d exits critical section\nINFO: writing: %d reading: %d   readers_waiting: %d writers_waiting: %d\n\n" ANSI_COLOR_RESET, curr_id, writing, reading, readers_waiting, writers_waiting);
    sem_up(mtx);;
    
    mythread_exit();

    return NULL;
}

int main(int argc, char *argv[])
{
    char *filename = NULL;
    char line[5];
    FILE *file = NULL;
    int i = 0;

    if(argc != 2)
    {
        printf("ERROR: Incorrect arguments\n");
        return -1;
    }

    filename = argv[1];
    file = fopen(filename, "r");
    if(file == NULL)
    {
        printf("ERROR: No input file\n");
        return -2;
    }

    readers_waiting = 0;
    writers_waiting = 0;
    reading = 0;
    writing = 0;

    reader_num = 0;
    writer_num = 0;

    threads_array = (thr_t *) malloc(100 * sizeof(thr_t));  // max 100 threads //
    mtx = sem_create(mtx, 1);
    lib_mtx = sem_create(lib_mtx, 1);

    read_sem = sem_create(read_sem, 0);
    write_sem = sem_create(write_sem, 0);

    mythreads_create(&threads_array[0], (void *) main, NULL);

    mythtreads_init();

    while (fgets(line, sizeof(line) ,file) != NULL)
    {
        if(line[0] == 'w')
        {
            custom_sleep(atoi(line+1));

            mythreads_create(&threads_array[i], (void *) writer, &writer_num);
            
            threads_array[i+1].id = 0;
            #ifdef DEBUG
            printf(ANSI_COLOR_GREEN "Writer created with sleep %d\n" ANSI_COLOR_RESET, atoi(line+1));
            #endif

            i++ ; // increase pos of array with ids //
        }
        else if(line[0] == 'r')
        {
            custom_sleep(atoi(line+1));
            mythreads_create(&threads_array[i], (void *) reader, &reader_num);
            
            #ifdef DEBUG
            printf(ANSI_COLOR_GREEN "Reader created with sleep %d\n" ANSI_COLOR_RESET, atoi(line+1));
            #endif                                                                                                                                                                                                      
            i++;  // increase pos of array with ids //
        }
    }

    for(i = 1; threads_array[i].id != 0; i++)
    {
        if(threads_array[i].id != -1)
        {
            mythreads_join(&threads_array[i]);
        }
    }
    free(threads_array);
    printf("Main exiting...\n");
}