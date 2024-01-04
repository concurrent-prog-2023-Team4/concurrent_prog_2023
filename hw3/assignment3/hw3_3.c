#include "hw3_3.h"

void *red_car(void *varg)
{
    enterMonitor(bridge_monitor);
    int red = reds_num;
    int red_num = red;
    #ifdef DEBUG
    printf(ANSI_COLOR_RED"Red (%d) created\n" ANSI_COLOR_RESET, red);
    #endif
    reds_num++;
    bridge->r_waiting++;
    exitMonitor(bridge_monitor);

    while (1)
    {
        enterMonitor(bridge_monitor);
        #ifdef DEBUG
        printf(ANSI_COLOR_RED"Red (%d) sees color: %c\n" ANSI_COLOR_RESET, red_num, bridge->color);
        #endif

        if(bridge->color == 'b')
        {
            bridge->r_down++;
            #ifdef DEBUG
            printf(ANSI_COLOR_RED "Red (%d) is waiting in bridge cause of color and b:%d and r:%d and on bridge %d\n" ANSI_COLOR_RESET, red_num, bridge->b_waiting, bridge->r_waiting, bridge->on_bridge);
            #endif
            wait(bridge_monitor);

            bridge->r_down--;
            exitMonitor(bridge_monitor);
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
                    wait(bridge_monitor);

                    bridge->r_down--;
                    exitMonitor(bridge_monitor);
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

            wait(bridge_monitor);
            bridge->r_down--;
            exitMonitor(bridge_monitor);
        }
    }

    // pernaei //
    bridge->r_waiting--;
    bridge->color = 'r';
    bridge->on_bridge++;
    bridge->blue_passed = 0;
    bridge->red_passed++;
    signal_all(bridge_monitor);
    exitMonitor(bridge_monitor);


    #ifdef DEBUG
    enterMonitor(bridge_monitor);
    printf(ANSI_COLOR_RED "Red (%d) goes into bridge and and b:%d and r:%d and on bridge %d. color: %c\n" ANSI_COLOR_RESET, red_num, bridge->b_waiting, bridge->r_waiting, bridge->on_bridge, bridge->color);
    exitMonitor(bridge_monitor);
    #endif

    sleep(6);
    
    enterMonitor(bridge_monitor);
    bridge->on_bridge--;
     #ifdef DEBUG
    printf(ANSI_COLOR_RED "Red (%d) is exiting bridge and b:%d and r:%d and on bridge %d. Reds passed: %d.\n" ANSI_COLOR_RESET, red_num, bridge->b_waiting, bridge->r_waiting, bridge->on_bridge, bridge->red_passed);
    #endif
    if(bridge->on_bridge == 0)
        bridge->color = '\0';
    
    signal_all(bridge_monitor);
    
    exitMonitor(bridge_monitor);

    pthread_exit(NULL);
}


void *blue_car(void *varg)
{
    
    enterMonitor(bridge_monitor);
    int blue = blues_num;
    #ifdef DEBUG
    printf(ANSI_COLOR_BLUE "Blue (%d) created\n" ANSI_COLOR_RESET, blue);
    #endif

    blues_num++;
    int blue_num = blue;
    bridge->b_waiting++;
    exitMonitor(bridge_monitor);

    while (1)
    {
        
        enterMonitor(bridge_monitor);
        #ifdef DEBUG
        printf(ANSI_COLOR_BLUE "Blue (%d) sees color: %c\n" ANSI_COLOR_RESET, blue_num, bridge->color);
        #endif
        if(bridge->color == 'r')
        {
            bridge->b_down++;
            #ifdef DEBUG
            printf(ANSI_COLOR_BLUE "Blue (%d) is waiting in bridge cause of color and b:%d and r:%d and on bridge %d\n" ANSI_COLOR_RESET, blue_num, bridge->b_waiting, bridge->r_waiting, bridge->on_bridge);
            #endif
            wait(bridge_monitor);

            bridge->b_down--;
            
            exitMonitor(bridge_monitor);
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
                    bridge->b_down++;
                    #ifdef DEBUG
                    printf(ANSI_COLOR_BLUE "Blue (%d) is waiting in bridge cause of red waiting to pass and b:%d and r:%d and on bridge %d\n" ANSI_COLOR_RESET, blue_num, bridge->b_waiting, bridge->r_waiting, bridge->on_bridge);
                    #endif
                    wait(bridge_monitor);

                    bridge->b_down--;
                    
                    exitMonitor(bridge_monitor);
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
            bridge->b_down++;
            #ifdef DEBUG
            printf(ANSI_COLOR_BLUE "Blue (%d) is waiting in bridge cause of max cars on it and b:%d and r:%d and on bridge %d\n" ANSI_COLOR_RESET, blue_num, bridge->b_waiting, bridge->r_waiting, bridge->on_bridge);
            #endif
            wait(bridge_monitor);
            bridge->b_down--;
            exitMonitor(bridge_monitor);
        }
    }
    
    bridge->b_waiting--;
    bridge->color = 'b';
    bridge->on_bridge++;
    bridge->red_passed = 0;
    bridge->blue_passed++;
    
    signal_all(bridge_monitor);
    exitMonitor(bridge_monitor);
    #ifdef DEBUG
    
    enterMonitor(bridge_monitor);
    printf(ANSI_COLOR_BLUE "Blue (%d) goes into bridge and b:%d and r:%d and on bridge %d. color: %c\n" ANSI_COLOR_RESET, blue_num, bridge->b_waiting, bridge->r_waiting, bridge->on_bridge, bridge->color);
    
    exitMonitor(bridge_monitor);
    #endif

    sleep(6);

    
    enterMonitor(bridge_monitor);
    bridge->on_bridge--;
    #ifdef DEBUG
    printf(ANSI_COLOR_BLUE "Blue (%d) is exiting bridge b:%d and r:%d and on bridge %d. Blues passed: %d\n" ANSI_COLOR_RESET, blue_num, bridge->b_waiting, bridge->r_waiting, bridge->on_bridge, bridge->blue_passed);
    #endif
    if(bridge->on_bridge == 0)
        bridge->color = '\0';

    signal_all(bridge_monitor);

    exitMonitor(bridge_monitor);

    pthread_exit(NULL);
}

#ifndef DEBUG
void *print_result(void *varg)
{
    bridge_t curbridge;
    int size = 0, i;

    while(1)
    {
        usleep(10);    // wait to see the next value //
        enterMonitor(bridge_monitor);
        curbridge = *bridge;
        
        exitMonitor(bridge_monitor);

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
    pthread_t ids[1000];
    int i = 0, j;

    if(argc != 3)
    {
        printf("ERROR: Incorrect arguments\n");
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
    bridge->b_down = 0;
    bridge->r_down = 0;
    filename = argv[1];

    file = fopen(filename, "r");
    if(filename == NULL)
    {
        printf("ERROR: No input file\n");
    }

    blues_num = 1;
    reds_num = 1;
    
    bridge_monitor = initMonitor(bridge_monitor);

    #ifndef DEBUG
    pthread_create(&ids[i], NULL, print_result, NULL);
    i++;
    #endif
    int red = 0, blue= 0;

    while (fgets(line, sizeof(line) ,file) != NULL)
    {
        if(line[0] == 'r')
        {
            red++;
            sleep(atoi(line+1));

            pthread_create(&ids[i], NULL, red_car, (void *) (&red));
            i++ ; // increase pos of array with ids //
        }
        else if(line[0] == 'b')
        {
            // enterMonitor(bridge_monitor);
            blue++;
            sleep(atoi(line+1));
            pthread_create(&ids[i], NULL, blue_car, (void *) (&blue));
            
            i++;  // increase pos of array with ids //
        }
    }

    for(j = 0; j < i; j++)
    {
        pthread_join(ids[j], NULL);
    }

    destroyMonitor(bridge_monitor);
    #ifdef DEBUG
    printf("Monitor bridge_monitor succesfully destroyed!\n");
    #endif

    return 0;
}
