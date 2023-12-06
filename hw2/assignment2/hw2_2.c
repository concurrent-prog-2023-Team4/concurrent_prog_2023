#include "hw2_3.h"

/*Red sees color:
  Red tries to be polite
  blockedddd
  */




void *red_car(void *varg)
{
    // bridge_t bridge;
    
    mysem_down(&mtx);
    int *red = (int *) varg;
    int red_num = *red;
    bridge->r_waiting++;
    mysem_up(&mtx);

    while (1)
    {
        // down
        //mysem_down(&mtx);
        //bridge = *bridge;
        //mysem_up(&mtx);
        mysem_down(&mtx);
        // mysem_down(&print_sem);
        printf("Red (%d) sees color: %c\n", red_num, bridge->color);
        // mysem_up(&print_sem);

        if (bridge->b_waiting > 0 && bridge->r_waiting > 0 && bridge->color == '\0' && bridge->blue_passed < 2*bridge->max_cars)
        {
            // mysem_down(&print_sem);
            printf("Red (%d) tries to be polite\n", red_num);
            // mysem_up(&print_sem);
            // mysem_up(&b_sem);
            mysem_up(&mtx);
            mysem_down(&r_sem);
        }

        else if(bridge->color == 'b')
        {
            // break; prepei down
            #ifdef DEBUG
            // mysem_down(&print_sem);
            printf(ANSI_COLOR_RED "Red (%d) is waiting in bridge cause of color and b:%d and r:%d and on bridge %d\n" ANSI_COLOR_RESET, red_num, bridge->b_waiting, bridge->r_waiting, bridge->on_bridge);
            // mysem_up(&print_sem);
            #endif
            mysem_up(&mtx);
            mysem_down(&r_sem);
        }
        else if(bridge->on_bridge < bridge->max_cars)
        {
            if(bridge->b_waiting > 0)
            {
                if(bridge->red_passed < 2*bridge->max_cars)
                {
                    // pernaei //
                    bridge->r_waiting--;
                    bridge->color = 'r';
                    bridge->on_bridge++;
                    bridge->blue_passed = 0;
                    bridge->red_passed++;
                    mysem_up(&mtx);
                    break;
                }
                else
                {
                    // down //
                    #ifdef DEBUG
                    // mysem_down(&print_sem);
                    printf(ANSI_COLOR_RED "Red (%d) is waiting in bridge cause of blue waiting to pass and and b:%d and r:%d and on bridge %d\n" ANSI_COLOR_RESET, red_num, bridge->b_waiting, bridge->r_waiting, bridge->on_bridge);
                    // mysem_up(&print_sem);
                    #endif
                    mysem_up(&mtx);
                    mysem_down(&r_sem); 
                }
            }
            else 
            {
                // pernaei //
                bridge->r_waiting--;
                bridge->color = 'r';
                bridge->on_bridge++;
                bridge->blue_passed = 0;
                bridge->red_passed++;
                mysem_up(&mtx);
                break;
            }
        }
        else
        {
            // down //
            #ifdef DEBUG
            // mysem_down(&print_sem);
            printf(ANSI_COLOR_RED "Red (%d) is waiting in bridge cause of max cars on it and and b:%d and r:%d and on bridge %d\n" ANSI_COLOR_RESET, red_num, bridge->b_waiting, bridge->r_waiting, bridge->on_bridge);
            // mysem_up(&print_sem);
            #endif
            mysem_up(&mtx);
            mysem_down(&r_sem);
        }
    }

    if(bridge->r_waiting > 0 && bridge->on_bridge < bridge->max_cars)
    {
        mysem_up(&r_sem);
    }
    // mysem_up(&mtx);

    #ifdef DEBUG
    mysem_down(&mtx);
    printf(ANSI_COLOR_RED "Red (%d) goes into bridge and and b:%d and r:%d and on bridge %d. color: %c\n" ANSI_COLOR_RESET, red_num, bridge->b_waiting, bridge->r_waiting, bridge->on_bridge, bridge->color);
    mysem_up(&mtx);
    #endif

    sleep(6);
    
    mysem_down(&mtx);
    bridge->on_bridge--;
     #ifdef DEBUG
    // mysem_down(&print_sem);
    printf(ANSI_COLOR_RED "Red (%d) is exiting bridge and b:%d and r:%d and on bridge %d. Reds passed: %d.\n" ANSI_COLOR_RESET, red_num, bridge->b_waiting, bridge->r_waiting, bridge->on_bridge, bridge->red_passed);
    // mysem_up(&print_sem);
    #endif
    if(bridge->on_bridge == 0)
        bridge->color = '\0';
    
    // if (bridge->b_waiting > 0 && bridge->r_waiting > 0 && bridge->color == '\0' && bridge->blue_passed < 2*bridge->max_cars)
    // {
    //     mysem_up(&b_sem);   //priority
    // }
    //else 
    //{
        if(bridge->b_waiting > 0)
        {
            mysem_up(&b_sem);
        }
        if(bridge->r_waiting > 0)
        {
            mysem_up(&r_sem);
        }
    //}
   
    mysem_up(&mtx);

    pthread_exit(NULL);
}


void *blue_car(void *varg)
{
    int *blue = (int *) varg;
    int blue_num = *blue;
    // bridge_t bridge;
    mysem_down(&mtx);
    bridge->b_waiting++;
    mysem_up(&mtx);

    while (1)
    {
        // down
        // mysem_down(&mtx);
        // bridge = *bridge;
        mysem_down(&mtx);
        // mysem_down(&print_sem);
        printf("Blue (%d) sees color: %c\n", blue_num, bridge->color);
        // mysem_up(&print_sem);
        if(bridge->color == 'r')
        {
            // break; prepei down
            #ifdef DEBUG
            // mysem_down(&print_sem);
            printf(ANSI_COLOR_BLUE "Blue (%d) is waiting in bridge cause of color and b:%d and r:%d and on bridge %d\n" ANSI_COLOR_RESET, blue_num, bridge->b_waiting, bridge->r_waiting, bridge->on_bridge);
            // mysem_up(&print_sem);
            #endif
            mysem_up(&mtx);
            mysem_down(&b_sem);
        }
        else if(bridge->on_bridge < bridge->max_cars)
        {
            if(bridge->r_waiting > 0)
            {
                if(bridge->blue_passed < 2*bridge->max_cars)
                {
                    // pernaei //
                    mysem_up(&mtx);
                    break;
                }
                else
                {
                    // down //
                    #ifdef DEBUG
                    // mysem_down(&print_sem);
                    printf(ANSI_COLOR_BLUE "Blue (%d) is waiting in bridge cause of red waiting to pass and b:%d and r:%d and on bridge %d\n" ANSI_COLOR_RESET, blue_num, bridge->b_waiting, bridge->r_waiting, bridge->on_bridge);
                    // mysem_up(&print_sem);
                    #endif
                    mysem_up(&mtx);
                    mysem_down(&b_sem);
                }
            }
            else
            {
                // pernaei //
                mysem_up(&mtx);
                break;
            }
        }
        else
        {
            // down //
            #ifdef DEBUG
            // mysem_down(&print_sem);
            printf(ANSI_COLOR_BLUE "Blue (%d) is waiting in bridge cause of max cars on it and b:%d and r:%d and on bridge %d\n" ANSI_COLOR_RESET, blue_num, bridge->b_waiting, bridge->r_waiting, bridge->on_bridge);
            // mysem_up(&print_sem);
            #endif
            mysem_up(&mtx);
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
    mysem_down(&mtx);
    printf(ANSI_COLOR_BLUE "Blue (%d) goes into bridge and b:%d and r:%d and on bridge %d. color: %c\n" ANSI_COLOR_RESET, blue_num, bridge->b_waiting, bridge->r_waiting, bridge->on_bridge, bridge->color);
    mysem_up(&mtx);
    #endif

    sleep(6);


    mysem_down(&mtx);
    bridge->on_bridge--;
    #ifdef DEBUG
    // mysem_down(&print_sem);
    printf(ANSI_COLOR_BLUE "Blue (%d) is exiting bridge b:%d and r:%d and on bridge %d. Blues passed: %d\n" ANSI_COLOR_RESET, blue_num, bridge->b_waiting, bridge->r_waiting, bridge->on_bridge, bridge->blue_passed);
    // mysem_up(&print_sem);
    #endif
    if(bridge->on_bridge == 0)
        bridge->color = '\0';

    // if (bridge->b_waiting > 0 && bridge->r_waiting > 0 && bridge->color == '\0' && bridge->blue_passed < 2*bridge->max_cars)
    // {
    //     mysem_up(&b_sem);   //priority
    // }
    //else 
    //{
        if(bridge->b_waiting > 0)
        {
            mysem_up(&b_sem);
        }
        if(bridge->r_waiting > 0)
        {
            mysem_up(&r_sem);
        }
    //}
    mysem_up(&mtx);

    pthread_exit(NULL);
}

#ifndef DEBUG
void *print_result(void *varg)
{
    bridge_t bridge;
    int size = 0, i;

    while(1)
    {
        usleep(10);    // wait to see the next value //

        mysem_down(&print_sem); // read data //
        bridge = *bridge;
        mysem_up(&print_sem);

        // if(bridge->b_waiting == 0 && bridge->r_waiting == 0 && bridge->on_bridge == 0)
        // {
        //     break;
        // }

        size = bridge->max_cars;
        // if(size == 0)
        // {
        //     sleep(1);
        //     system("sl");
        // }

        printf("\r");
        printf(ANSI_COLOR_RED "r:%d "ANSI_COLOR_RESET , bridge->r_waiting);
        for(i = 0; i < size; i++)
        {
            if(i < bridge->on_bridge)
            {
                if(bridge->color == 'r')
                {
                    printf(ANSI_COLOR_RED "R " ANSI_COLOR_RESET);
                }
                else if(bridge->color == 'b')
                {
                    printf(ANSI_COLOR_BLUE "B " ANSI_COLOR_RESET);
                }
            }
            else
            {
                printf("_ ");
            }
        }
        printf(ANSI_COLOR_BLUE " b:%d" ANSI_COLOR_RESET, bridge->b_waiting);

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

    int red = 0, blue= 0;

    while (fgets(line, sizeof(line) ,file) != NULL)
    {
        if(line[0] == 'r')
        {
            red++;
            sleep(atoi(line+1));
            pthread_create(&ids[i], NULL, red_car, (void *) (&red));

            #ifdef DEBUG
            mysem_down(&print_sem);
            printf(ANSI_COLOR_RED "Red (%d) created with sleep %d\n" ANSI_COLOR_RESET, red, atoi(line+1));
            mysem_up(&print_sem);
            #endif

            i++ ;
        }
        else if(line[0] == 'b')
        {
            blue++;
            sleep(atoi(line+1));
            pthread_create(&ids[i], NULL, blue_car, (void *) (&blue));
            
            #ifdef DEBUG
            mysem_down(&print_sem);
            printf(ANSI_COLOR_BLUE "Blue (%d) created with sleep %d\n" ANSI_COLOR_RESET, blue, atoi(line+1));
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
