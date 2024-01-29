#include "writers_readers.h"

// void *writer()
// {
//     int curr_id;

//     mysem_down(&mtx);
//     curr_id = writer_num;
//     writer_num++;
//     mysem_up(&mtx);

//     // while(1)
//     // {
//         mysem_down(&mtx);
//         if (reading + writing > 0) 
//         {
//             writers_waiting++;
//             mysem_up(&mtx);;
//             mysem_down(&write_sem);
//         }
//         else 
//         {
//             // writing++;
//             mysem_up(&mtx);;
//             // break;
//         }
//         writing++;
//     // }
//     /* write */
//     printf(ANSI_COLOR_RED "Writer %d enters critical section\nwriting: %d reading: %d\nreaders_waiting: %d writers_waiting: %d\n\n" ANSI_COLOR_RESET, curr_id, writing, reading, readers_waiting, writers_waiting);
//     sleep(2);

//     mysem_down(&mtx);
//     writing--;
//     if (readers_waiting > 0) 
//     {
//         readers_waiting--;
//         // reading++; 
//         mysem_up(&read_sem);
//         printf(ANSI_COLOR_RED "Writer %d exits critical section\nwriting: %d reading: %d\nreaders_waiting: %d writers_waiting: %d\n\n" ANSI_COLOR_RESET, curr_id, writing, reading, readers_waiting, writers_waiting);
//     }
//     else 
//     {
//         if (writers_waiting > 0) 
//         {
//             writers_waiting--;
//             // writing++;
//             mysem_up(&write_sem);
//         }
//         printf(ANSI_COLOR_RED "Writer %d exits critical section\nwriting: %d reading: %d\nreaders_waiting: %d writers_waiting: %d\n\n" ANSI_COLOR_RESET, curr_id, writing, reading, readers_waiting, writers_waiting);
//         mysem_up(&mtx);
//     }

//     pthread_exit(NULL);
// }

// void *reader()
// {
//     int curr_id;

//     mysem_down(&mtx);
//     curr_id = reader_num;
//     reader_num++;
//     mysem_up(&mtx);;

//     // while(1)
//     // {
//         mysem_down(&mtx);
//         if (writing + writers_waiting > 0) 
//         {
//             readers_waiting++;
//             mysem_up(&mtx);;
//             mysem_down(&read_sem);
//             if (readers_waiting > 0) 
//             {
//                 readers_waiting--;
//                 // reading++;
//                 mysem_up(&read_sem);
//                 // break;
//             }
//             else 
//             {
//                 mysem_up(&mtx);
//                 // break;
//             }
//         }
//         else 
//         {
//             // reading++;
//             mysem_up(&mtx);
//             // break;
//         }

//         reading++;  
//     // }
//     /* read */
//     printf(ANSI_COLOR_BLUE "Reader %d enters critical section\nwriting: %d reading: %d\nreaders_waiting: %d writers_waiting: %d\n\n" ANSI_COLOR_RESET, curr_id, writing, reading, readers_waiting, writers_waiting);
//     sleep(2);

//     mysem_down(&mtx);
//     reading--;
//     if ( (reading == 0) && (writers_waiting > 0) )
//     {
//         writers_waiting--;
//         writing++;
//         mysem_up(&write_sem);
//     }
//     printf(ANSI_COLOR_BLUE "Reader %d exits critical section\nwriting: %d reading: %d\nreaders_waiting: %d writers_waiting: %d\n\n" ANSI_COLOR_RESET, curr_id, writing, reading, readers_waiting, writers_waiting);
//     mysem_up(&mtx);;
    
//     pthread_exit(NULL);

// }

void *writer()
{
    int curr_id;

    mysem_down(&mtx);
    curr_id = writer_num;
    writer_num++;
    mysem_up(&mtx);

    // while(1)
    // {
        mysem_down(&mtx);
        if (reading + writing > 0) 
        {
            writers_waiting++;
            mysem_up(&mtx);
            mysem_down(&write_sem);
        }
        else 
        {
            writing++;
            mysem_up(&mtx);;
            // break;
        }
        // writing++;
    // }
    /* write */
    printf(ANSI_COLOR_RED "Writer %d enters critical section\nwriting: %d reading: %d\nreaders_waiting: %d writers_waiting: %d\n\n" ANSI_COLOR_RESET, curr_id, writing, reading, readers_waiting, writers_waiting);
    sleep(2);

    mysem_down(&mtx);
    writing--;
    if (readers_waiting > 0) 
    {
        readers_waiting--;
        reading++; 
        mysem_up(&read_sem);
        printf(ANSI_COLOR_RED "Writer %d exits critical section\nwriting: %d reading: %d\nreaders_waiting: %d writers_waiting: %d\n\n" ANSI_COLOR_RESET, curr_id, writing, reading, readers_waiting, writers_waiting);
    }
    else 
    {
        if (writers_waiting > 0) 
        {
            writers_waiting--;
            writing++;
            mysem_up(&write_sem);
        }
        printf(ANSI_COLOR_RED "Writer %d exits critical section\nwriting: %d reading: %d\nreaders_waiting: %d writers_waiting: %d\n\n" ANSI_COLOR_RESET, curr_id, writing, reading, readers_waiting, writers_waiting);
        mysem_up(&mtx);
    }

    pthread_exit(NULL);
}

void *reader()
{
    int curr_id;

    mysem_down(&mtx);
    curr_id = reader_num;
    reader_num++;
    mysem_up(&mtx);;

    // while(1)
    // {
        mysem_down(&mtx);
        if (writing + writers_waiting > 0) 
        {
            readers_waiting++;
            mysem_up(&mtx);;
            mysem_down(&read_sem);
            if (readers_waiting > 0) 
            {
                readers_waiting--;
                reading++;
                mysem_up(&read_sem);
                // break;
            }
            else 
            {
                mysem_up(&mtx);
                // break;
            }
        }
        else 
        {
            reading++;
            mysem_up(&mtx);
            // break;
        }

        // reading++;  
    // }
    /* read */
    printf(ANSI_COLOR_BLUE "Reader %d enters critical section\nwriting: %d reading: %d\nreaders_waiting: %d writers_waiting: %d\n\n" ANSI_COLOR_RESET, curr_id, writing, reading, readers_waiting, writers_waiting);
    sleep(2);

    mysem_down(&mtx);
    reading--;
    if ( (reading == 0) && (writers_waiting > 0) )
    {
        writers_waiting--;
        writing++;
        mysem_up(&write_sem);
    }
    printf(ANSI_COLOR_BLUE "Reader %d exits critical section\nwriting: %d reading: %d\nreaders_waiting: %d writers_waiting: %d\n\n" ANSI_COLOR_RESET, curr_id, writing, reading, readers_waiting, writers_waiting);
    mysem_up(&mtx);;
    
    pthread_exit(NULL);

}

int main(int argc, char *argv[])
{   
    char *filename = NULL;
    char line[5];
    FILE *file = NULL;
    int i = 0;
    pthread_t threads_array[100];

    if(argc != 2)
    {
        printf("ERROR: Incorrect arguments\n");
        return -1;
    }

    filename = argv[1];
    file = fopen(filename, "r");
    if(filename == NULL)
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

    // threads_array = (thr_t *) malloc(100 * sizeof(thr_t));  // max 100 threads //
    mysem_create(&mtx);
    mysem_init(&mtx, 1);
    // mysem_create(lib_mtx);
    // mysem_init(lib_mtx, 1);

    mysem_create(&read_sem);
    mysem_create(&write_sem);
    mysem_init(&read_sem, 0);
    mysem_init(&write_sem, 0);

    // mythreads_create(&threads_array[0], (void *) main, NULL);

    // mythtreads_init();

    while (fgets(line, sizeof(line) ,file) != NULL)
    {
        if(line[0] == 'w')
        {
            // custom_sleep(atoi(line+1));
            sleep(atoi(line+1));

            // mythreads_create(&threads_array[i], (void *) writer, &writer_num);
            
            pthread_create(&threads_array[i], NULL, (void *) writer, &writer_num);
            // threads_array[i+1].id = 0;
            #ifdef DEBUG
            printf(ANSI_COLOR_RED "Writer created with sleep %d\n" ANSI_COLOR_RESET, atoi(line+1));
            #endif

            i++ ; // increase pos of array with ids //
        }
        else if(line[0] == 'r')
        {
            // custom_sleep(atoi(line+1));
            sleep(atoi(line+1));
            // mythreads_create(&threads_array[i], (void *) reader, &reader_num);
            pthread_create(&threads_array[i], NULL, (void *) reader, &reader_num);
            
            #ifdef DEBUG
            printf(ANSI_COLOR_BLUE "Reader created with sleep %d\n" ANSI_COLOR_RESET, atoi(line+1));
            #endif                                                                                                                                                                                                      
            i++;  // increase pos of array with ids //
        }
    }

    sleep(20);

    for(i = 0; i < 6; i++)
    {
        pthread_join(threads_array[i], NULL);
    }
}