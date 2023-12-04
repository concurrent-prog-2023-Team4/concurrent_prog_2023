#include "hw2_3.h"

void *red_car(void *varg)
{
    bridge_t cur_bridge;

    mysem_down(&mtx);
    bridge->r_waiting++;
    mysem_up(&mtx);

    while (1)
    {
        // down
        mysem_down(&mtx);
        cur_bridge = *bridge;
        mysem_up(&mtx);

        if(cur_bridge.color == 'b')
        {
            // break; prepei down
            #ifdef DEBUG
            mysem_down(&print_sem);
            printf(ANSI_COLOR_RED "Red is waiting in bridge cause of color and b:%d and r:%d\n" ANSI_COLOR_RESET, cur_bridge.b_waiting, cur_bridge.r_waiting);
            mysem_up(&print_sem);
            #endif

            mysem_down(&r_sem);
        }
        else if(cur_bridge.on_bridge < cur_bridge.max_cars)
        {
            if(cur_bridge.b_waiting > 0)
            {
                if(cur_bridge.red_passed < 2*cur_bridge.max_cars)
                {
                    // pernaei //

                    break;
                }
                else
                {
                    // down //
                    #ifdef DEBUG
                    mysem_down(&print_sem);
                    printf(ANSI_COLOR_RED "Red is waiting in bridge cause of blue waiting to passand b:%d and r:%d\n" ANSI_COLOR_RESET , cur_bridge.b_waiting, cur_bridge.r_waiting);
                    mysem_up(&print_sem);
                    #endif
                    mysem_down(&r_sem);
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
            #ifdef DEBUG
            mysem_down(&print_sem);
            printf(ANSI_COLOR_RED "Red is waiting in bridge cause of max cars on it and b:%d and r:%d\n" ANSI_COLOR_RESET, cur_bridge.b_waiting, cur_bridge.r_waiting);
            mysem_up(&print_sem);
            #endif

            mysem_down(&r_sem);
        }

    }
    mysem_down(&mtx);
    bridge->r_waiting--;
    bridge->color = 'r';
    bridge->on_bridge++;
    bridge->blue_passed = 0;
    bridge->red_passed++;
    if(bridge->r_waiting > 0 && bridge->on_bridge < bridge->max_cars)
    {
        mysem_up(&r_sem);
    }
    mysem_up(&mtx);

    #ifdef DEBUG
    mysem_down(&print_sem);
    printf(ANSI_COLOR_RED "Red goes into bridge and on_bridge: %d\n" ANSI_COLOR_RESET, cur_bridge.on_bridge);
    mysem_up(&print_sem);
    #endif

    sleep(3);
    #ifdef DEBUG
    mysem_down(&print_sem);
    printf(ANSI_COLOR_RED "Red is exiting bridge\n" ANSI_COLOR_RESET);
    mysem_up(&print_sem);
    #endif
    
    mysem_down(&mtx);
    bridge->on_bridge--;
    if(bridge->on_bridge == 0)
        bridge->color = '\0';

    if(bridge->b_waiting > 0)
    {
        mysem_up(&b_sem);
    }
    if(bridge->r_waiting > 0)
    {
        mysem_up(&r_sem);
    }
    mysem_up(&mtx);

    pthread_exit(NULL);
}


void *blue_car(void *varg)
{
    bridge_t cur_bridge;
    mysem_down(&mtx);
    bridge->b_waiting++;
    mysem_up(&mtx);

    while (1)
    {
        // down
        mysem_down(&mtx);
        cur_bridge = *bridge;
        mysem_up(&mtx);

        if(cur_bridge.color == 'r')
        {
            // break; prepei down
            #ifdef DEBUG
            mysem_down(&print_sem);
            printf(ANSI_COLOR_BLUE "Blue is waiting in bridge cause of color and b:%d and r:%d\n" ANSI_COLOR_RESET, cur_bridge.b_waiting, cur_bridge.r_waiting);
            mysem_up(&print_sem);
            #endif

            mysem_down(&b_sem);
        }
        else if(cur_bridge.on_bridge < cur_bridge.max_cars)
        {
            if(cur_bridge.r_waiting > 0)
            {
                if(cur_bridge.blue_passed < 2*cur_bridge.max_cars)
                {
                    // pernaei //
                    break;
                }
                else
                {
                    // down //
                    #ifdef DEBUG
                    mysem_down(&print_sem);
                    printf(ANSI_COLOR_BLUE "Blue is waiting in bridge cause of red waiting to pass and b:%d and r:%d\n" ANSI_COLOR_RESET, cur_bridge.b_waiting, cur_bridge.r_waiting);
                    mysem_up(&print_sem);
                    #endif

                    mysem_down(&b_sem);
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
            #ifdef DEBUG
            mysem_down(&print_sem);
            printf(ANSI_COLOR_BLUE "Blue is waiting in bridge cause of max cars on it and b:%d and r:%d\n" ANSI_COLOR_RESET, cur_bridge.b_waiting, cur_bridge.r_waiting);
            mysem_up(&print_sem);
            #endif

            mysem_down(&b_sem);
        }

    }
    mysem_down(&mtx);
    bridge->b_waiting--;
    bridge->color = 'b';
    bridge->on_bridge++;
    bridge->red_passed = 0;
    bridge->blue_passed++;
    if (bridge->on_bridge < bridge->max_cars && bridge->b_waiting > 0)
    {
        mysem_up(&b_sem);
    }
    mysem_up(&mtx);

    #ifdef DEBUG
    mysem_down(&print_sem);
    printf(ANSI_COLOR_BLUE "Blue goes into bridge\n" ANSI_COLOR_RESET);
    mysem_up(&print_sem);
    #endif

    sleep(3);

    #ifdef DEBUG
    mysem_down(&print_sem);
    printf(ANSI_COLOR_BLUE "Blue is exiting bridge\n"ANSI_COLOR_RESET);
    mysem_up(&print_sem);
    #endif
    
    mysem_down(&mtx);
    
    bridge->on_bridge--;
    if(bridge->on_bridge == 0)
        bridge->color = '\0';

    if(bridge->b_waiting > 0)
    {
        mysem_up(&b_sem);
    }
    if(bridge->r_waiting > 0)
    {
        mysem_up(&r_sem);
    }
    mysem_up(&mtx);

    pthread_exit(NULL);
}

#ifndef DEBUG
void *print_result(void *varg)
{
    bridge_t cur_bridge;
    int size = 0, i;

    while(1)
    {
        usleep(10);    // wait to see the next value //

        mysem_down(&print_sem); // read data //
        cur_bridge = *bridge;
        mysem_up(&print_sem);

        // if(cur_bridge.b_waiting == 0 && cur_bridge.r_waiting == 0 && cur_bridge.on_bridge == 0)
        // {
        //     break;
        // }

        size = cur_bridge.max_cars;
        // if(size == 0)
        // {
        //     sleep(1);
        //     system("sl");
        // }

        printf("\r");
        printf(ANSI_COLOR_RED "r:%d "ANSI_COLOR_RESET , cur_bridge.r_waiting);
        for(i = 0; i < size; i++)
        {
            if(i < cur_bridge.on_bridge)
            {
                if(cur_bridge.color == 'r')
                {
                    printf(ANSI_COLOR_RED "R " ANSI_COLOR_RESET);
                }
                else if(cur_bridge.color == 'b')
                {
                    printf(ANSI_COLOR_BLUE "B " ANSI_COLOR_RESET);
                }
            }
            else
            {
                printf("_ ");
            }
        }
        printf(ANSI_COLOR_BLUE " b:%d" ANSI_COLOR_RESET, cur_bridge.b_waiting);

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
    pthread_t ids[100];
    int i = 0, j;

    if(argc != 3)
    {
        // printf("ERROR: Incorrect arguments\n");
        return -1;
    }

    bridge = (bridge_t*) calloc(1, sizeof(bridge_t));
    bridge->b_waiting = 0;
    bridge->blue_passed = 0;    
    bridge->red_passed = 0;
    bridge->max_cars = 0;
    bridge->r_waiting = 0;
    bridge->color = '\0';
    bridge->max_cars = atoi(argv[2]);   // first filename and then max_number //
    filename = argv[1];

    exit_val = 0;


    file = fopen(filename, "r");
    if(filename == NULL)
    {
        // printf("ERROR: No input file\n");
    }
    mysem_create(&b_sem);
    mysem_create(&r_sem);
    mysem_create(&mtx);
    mysem_create(&print_sem);

    mysem_init(&mtx, 1);
    mysem_init(&r_sem, 0);
    mysem_init(&b_sem, 0);
    mysem_init(&print_sem, 1);

    #ifndef DEBUG
    pthread_create(&ids[i], NULL, print_result, NULL);
    i++;
    #endif

    while (fgets(line, sizeof(line) ,file) != NULL)
    {
        if(line[0] == 'r')
        {
            sleep(atoi(line+1));
            pthread_create(&ids[i], NULL, red_car, NULL);

            #ifdef DEBUG
            mysem_down(&print_sem);
            printf(ANSI_COLOR_RED "Red created with sleep %d\n" ANSI_COLOR_RESET, atoi(line+1));
            mysem_up(&print_sem);
            #endif

            i++ ;
        }
        else if(line[0] == 'b')
        {
            sleep(atoi(line+1));
            pthread_create(&ids[i], NULL, blue_car, NULL);
            #ifdef DEBUG
            mysem_down(&print_sem);
            printf(ANSI_COLOR_BLUE "Blue created with sleep %d\n" ANSI_COLOR_RESET, atoi(line+1));
            mysem_up(&print_sem);
            #endif
            i++;
        }
    }

    exit_val = 1;
    for(j = 0; j < i; j++)
    {
        pthread_join(ids[j], NULL);
    }
}
