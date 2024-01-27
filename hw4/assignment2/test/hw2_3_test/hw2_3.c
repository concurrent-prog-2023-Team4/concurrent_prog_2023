#include "hw2_3.h"

void *red_car(void *varg)
{
    sem_down(mtx);
    int red_num = red_id;
    threads_array[current_thread].id = red_id;
    red_id++;
    bridge->r_waiting++;
    sem_up(mtx);

    while (1)
    {
        sem_down(mtx);
        #ifdef DEBUG
        printf("Red (%d) sees color: %c\n", red_num, bridge->color);
        #endif

        if (bridge->b_waiting > 0 && bridge->r_waiting > 0 && bridge->color == '\0' && bridge->blue_passed < 2*bridge->max_cars)
        {
            #ifdef DEBUG
            printf("Red (%d) tries to be polite\n", red_num);
            #endif
            sem_up(mtx);
            sem_down(r_sem);
        }

        else if(bridge->color == 'b')
        {
            bridge->r_down++;
            #ifdef DEBUG
            printf(ANSI_COLOR_RED "Red (%d) is waiting in bridge cause of color and b:%d and r:%d and on bridge %d\n" ANSI_COLOR_RESET, red_num, bridge->b_waiting, bridge->r_waiting, bridge->on_bridge);
            #endif
            sem_up(mtx);
            sem_down(r_sem);

            bridge->r_down--;
        }
        else if(bridge->on_bridge < bridge->max_cars)
        {
            if(bridge->b_waiting > 0)
            {
                if(bridge->red_passed < 2*bridge->max_cars)
                {   
                    break;
                }
                else
                {
                    // down //
                    bridge->r_down++;
                    #ifdef DEBUG
                    printf(ANSI_COLOR_RED "Red (%d) is waiting in bridge cause of blue waiting to pass and and b:%d and r:%d and on bridge %d\n" ANSI_COLOR_RESET, red_num, bridge->b_waiting, bridge->r_waiting, bridge->on_bridge);
                    #endif
                    sem_up(mtx);
                    sem_down(r_sem); 

                    bridge->r_down--;
                }
            }
            else 
            {
                // pernaei //
                break;
            }
        }
        else
        {
            // down //
            bridge->r_down++;
            #ifdef DEBUG
            printf(ANSI_COLOR_RED "Red (%d) is waiting in bridge cause of max cars on it and and b:%d and r:%d and on bridge %d\n" ANSI_COLOR_RESET, red_num, bridge->b_waiting, bridge->r_waiting, bridge->on_bridge);
            #endif
            sem_up(mtx);
            sem_down(r_sem);

            bridge->r_down--;
        }
    }

    // pernaei //
    bridge->r_waiting--;
    bridge->color = 'r';
    bridge->on_bridge++;
    bridge->blue_passed = 0;
    bridge->red_passed++;
    sem_up(mtx);
               

    if(bridge->r_down > 0 && bridge->on_bridge < bridge->max_cars)
    {
        if(sem_up(r_sem) == 0)
            printf("sem r_sem lost up\n");
    }

    #ifdef DEBUG
    sem_down(mtx);
    printf(ANSI_COLOR_RED "Red (%d) goes into bridge and and b:%d and r:%d and on bridge %d. color: %c\n" ANSI_COLOR_RESET, red_num, bridge->b_waiting, bridge->r_waiting, bridge->on_bridge, bridge->color);
    sem_up(mtx);
    #endif

    // sleep(6);
    custom_sleep(6);
    
    sem_down(mtx);
    bridge->on_bridge--;
     #ifdef DEBUG
    printf(ANSI_COLOR_RED "Red (%d) is exiting bridge and b:%d and r:%d and on bridge %d. Reds passed: %d.\n" ANSI_COLOR_RESET, red_num, bridge->b_waiting, bridge->r_waiting, bridge->on_bridge, bridge->red_passed);
    #endif
    if(bridge->on_bridge == 0)
        bridge->color = '\0';
    
    if(bridge->b_down > 0)
    {
        if(sem_up(b_sem) == 0)
            printf("sem b_sem lost up 1\n");
    }
    if(bridge->r_down > 0)
    {
        if(sem_up(r_sem) == 0)
            printf("sem r_sem lost up\n");
    }

    sem_up(mtx);

    
    while(1);
    // pthread_exit(NULL);
}


void *blue_car(void *varg)
{
    sem_down(mtx);
    int blue_num = blue_id;
    threads_array[current_thread].id = blue_id;
    blue_id--;
    bridge->b_waiting++;
    sem_up(mtx);

    while (1)
    {
        sem_down(mtx);
        #ifdef DEBUG
        printf("Blue (%d) sees color: %c\n", blue_num, bridge->color);
        #endif
        if(bridge->color == 'r')
        {
            bridge->b_down++;
            #ifdef DEBUG
            printf(ANSI_COLOR_BLUE "Blue (%d) is waiting in bridge cause of color and b:%d and r:%d and on bridge %d\n" ANSI_COLOR_RESET, blue_num, bridge->b_waiting, bridge->r_waiting, bridge->on_bridge);
            #endif
            sem_up(mtx);
            sem_down(b_sem);

            bridge->b_down--;
        }
        else if(bridge->on_bridge < bridge->max_cars)
        {
            if(bridge->r_waiting > 0)
            {
                if(bridge->blue_passed < 2*bridge->max_cars)
                {
           
                    break;
                }
                else
                {
                    // down //
                    bridge->b_down++;
                    #ifdef DEBUG
                    printf(ANSI_COLOR_BLUE "Blue (%d) is waiting in bridge cause of red waiting to pass and b:%d and r:%d and on bridge %d\n" ANSI_COLOR_RESET, blue_num, bridge->b_waiting, bridge->r_waiting, bridge->on_bridge);
                    #endif
                    sem_up(mtx);
                    sem_down(b_sem);
                    
                    bridge->b_down--;
                }
            }
            else
            {
                // pernaei //
                break;
            }
        }
        else
        {
            // down //
            bridge->b_down++;
            #ifdef DEBUG
            printf(ANSI_COLOR_BLUE "Blue (%d) is waiting in bridge cause of max cars on it and b:%d and r:%d and on bridge %d\n" ANSI_COLOR_RESET, blue_num, bridge->b_waiting, bridge->r_waiting, bridge->on_bridge);
            #endif
            sem_up(mtx);
            sem_down(b_sem);
            
            bridge->b_down--;
            
        }
    }
    
    bridge->b_waiting--;
    bridge->color = 'b';
    bridge->on_bridge++;
    bridge->red_passed = 0;
    bridge->blue_passed++;
    if (bridge->on_bridge < bridge->max_cars && bridge->b_down > 0)
    {
        if(sem_up(b_sem) == 0)
            printf("Sem b_sem lost up 2\n");
    }
    sem_up(mtx);

    #ifdef DEBUG
    sem_down(mtx);
    printf(ANSI_COLOR_BLUE "Blue (%d) goes into bridge and b:%d and r:%d and on bridge %d. color: %c\n" ANSI_COLOR_RESET, blue_num, bridge->b_waiting, bridge->r_waiting, bridge->on_bridge, bridge->color);
    sem_up(mtx);
    #endif

    // sleep(6);
    custom_sleep(6);

    sem_down(mtx);
    bridge->on_bridge--;
    #ifdef DEBUG
    printf(ANSI_COLOR_BLUE "Blue (%d) is exiting bridge b:%d and r:%d and on bridge %d. Blues passed: %d\n" ANSI_COLOR_RESET, blue_num, bridge->b_waiting, bridge->r_waiting, bridge->on_bridge, bridge->blue_passed);
    #endif
    if(bridge->on_bridge == 0)
        bridge->color = '\0';

    if(bridge->b_down > 0)
    {
        if(sem_up(b_sem) == 0)
            printf("semaphore b_sem lost up 3\n");
    }
    if(bridge->r_down > 0)
    {
        if(sem_up(r_sem) == 0)
            printf("semaphore r_sem lost up\n");
    }


    sem_up(mtx);
    while (1) 
    {
        
    }
    // pthread_exit(NULL);
}

#ifndef DEBUG
void *print_result(void *varg)
{
    bridge_t curbridge;
    int size = 0, i;

    while(1)
    {
        usleep(10);    // wait to see the next value //

        sem_down(print_sem); // read data //
        curbridge = *bridge;
        sem_up(print_sem);

        size = curbridge.max_cars;

        printf("\r");
        printf(ANSI_COLOR_RED "r:%d "ANSI_COLOR_RESET , curbridge.r_waiting);
        for(i = 0; i < size; i++)
        {
            if(i < curbridge.on_bridge)
            {
                if(curbridge.color == 'r')
                {
                    printf(ANSI_COLOR_RED "R " ANSI_COLOR_RESET);
                }
                else if(curbridge.color == 'b')
                {
                    printf(ANSI_COLOR_BLUE "B " ANSI_COLOR_RESET);
                }
            }
            else
            {
                printf("_ ");
            }
        }
        printf(ANSI_COLOR_BLUE " b:%d" ANSI_COLOR_RESET, curbridge.b_waiting);

        fflush(stdout);
    }
    pthread_exit(NULL);

}
#endif

int main(int argc, char *argv[])
{
    FILE *file;
    char line[5];
    char *filename;
    int i = 0, j;

    if(argc != 3)
    {
        printf("ERROR: Incorrect arguments\n");
        return -1;
    }

    red_id = 2;
    blue_id = -1;

    bridge = (bridge_t*) calloc(1, sizeof(bridge_t));
    bridge->b_waiting = 0;
    bridge->blue_passed = 0;    
    bridge->red_passed = 0;
    bridge->max_cars = 0;
    bridge->r_waiting = 0;
    bridge->color = '\0';
    bridge->max_cars = atoi(argv[2]);   // first filename and then max_number //
    bridge->b_down = 0;
    bridge->r_down = 0;
    filename = argv[1];

    // create threads array //
    threads_array = (thr_t *) malloc(100 * sizeof(thr_t));

    file = fopen(filename, "r");
    if(filename == NULL)
    {
        printf("ERROR: No input file\n");
    }
    b_sem = sem_create(b_sem, 0);
    r_sem = sem_create(r_sem, 0);
    mtx = sem_create(mtx, 1);
    print_sem = sem_create(print_sem, 1);

    lib_mtx = sem_create(lib_mtx, 1);

    thread_ids = 1;
    mythreads_create(&threads_array[0], (void *) main, NULL);

    // mysem_init(&mtx, 1);
    // mysem_init(&r_sem, 0);
    // mysem_init(&b_sem, 0);
    // mysem_init(&print_sem, 1);

    #ifndef DEBUG
    
    pthread_create(&ids[i], NULL, print_result, NULL);
    i++;
    #endif
    int red = 0, blue = 0;

    mythtreads_init();

    while (fgets(line, sizeof(line) ,file) != NULL)
    {
        if(line[0] == 'r')
        {
             
            // sleep(atoi(line+1));
            custom_sleep(atoi(line+1));

            mythreads_create(&threads_array[i], (void *) red_car, NULL);
            
            threads_array[i+1].id = 0;
            #ifdef DEBUG
            sem_down(print_sem);
            printf(ANSI_COLOR_RED "Red (%d) created with sleep %d\n" ANSI_COLOR_RESET, red, atoi(line+1));
            sem_up(print_sem);
            #endif

            i++ ; // increase pos of array with ids //
        }
        else if(line[0] == 'b')
        {
            
            // sleep(atoi(line+1));
            custom_sleep(atoi(line+1));
            mythreads_create(&threads_array[i], (void *) blue_car, NULL);
            
            #ifdef DEBUG
            sem_down(print_sem);
            printf(ANSI_COLOR_BLUE "Blue (%d) created with sleep %d\n" ANSI_COLOR_RESET, blue, atoi(line+1));
            sem_up(print_sem);
            #endif                                                                                                                                                                                                      
            i++;  // increase pos of array with ids //
        }
    }

    while (1) {
        
    }
    for(j = 0; j < i; j++)
    {
        mythreads_join(&threads_array[j]);
    }

    // if(sem_destroy(&b_sem))
    //     printf("Semaphore b_sem destroyed succesfully!\n");
    // if (sem_destroy(&r_sem))
    //     printf("Semaphore r_sem destroyed succesfully!\n");
    // if (sem_destroy(&mtx))
    //     printf("Semaphore mtx destroyed succesfully!\n");
    // if (sem_destroy(&print_sem))
    //     printf("Semaphore print_sem destroyed succesfully!\n");
}
