#include "hw3_4.h"

void *train_function()
{
    while(1)
    {   
        
        // mysem_down(&train_sem);
        enterMonitor(train_mon);
        wait(train_mon);
        exitMonitor(train_mon);

        enterMonitor(print);
        printf(ANSI_COLOR_RED "Train starts!\n" ANSI_COLOR_RESET);
        exitMonitor(print);
        
        sleep(3);   // time of ride //

        enterMonitor(print);
        printf(ANSI_COLOR_RED "Train stops!\n" ANSI_COLOR_RESET);
        exitMonitor(print);

        enterMonitor(pass_ride);
        signal_all(pass_ride);
        exitMonitor(pass_ride);

        enterMonitor(train_mon);
        wait(train_mon);
        exitMonitor(train_mon);

        enterMonitor(pass_wait);
        train->on_train = 0;
        train->pass_exits = 0;
        signal_all(pass_wait);
        exitMonitor(pass_wait);

        if(train->exit == 1)
        {
            break;
        }
    }

    pthread_exit(NULL);
}

void *passengers_function(void *varg)
{   
    int *flag = (int *) varg;
    int inform_exit = 0;

    if(flag != NULL)
    {
        inform_exit = 1;
    }

    enterMonitor(pass_wait);
    long int num = pass_num;
    pass_num++;
    printf("Thread with id %ld created!\n", num);  
    exitMonitor(pass_wait);

    while(1)
    {
        enterMonitor(pass_wait);
        if(train->on_train == train->max_passengers)
        {
           
            printf(ANSI_COLOR_BLUE "Passenger with id %ld waiting\n" ANSI_COLOR_RESET , num);
            wait(pass_wait);
            exitMonitor(pass_wait);
        }
        else
        {       
            train->on_train++;
            train->passengers[train->on_train-1] = pthread_self();
            if(train->max_passengers == train->on_train)
            {   
                exitMonitor(pass_wait);
                enterMonitor(train_mon);
                signal(train_mon);
                exitMonitor(train_mon);
            }
            else
            {
                exitMonitor(pass_wait);
            }
            enterMonitor(print);
            printf("Passenger with %ld gets on train\n", num);
            exitMonitor(print);

            break;
        }
    }
    enterMonitor(pass_ride);
    wait(pass_ride);
    train->pass_exits++;
    if(train->pass_exits == train->max_passengers)
    {
        printf("Passenger %ld exits\n", num);
        exitMonitor(pass_ride);
        enterMonitor(train_mon);
        signal(train_mon);
        exitMonitor(train_mon);
    }
    else
    {
        printf("Passenger %ld exits\n", num);
        exitMonitor(pass_ride);
    }
    
    if(inform_exit)
    {
        train->exit = 1;
    }
    pthread_exit(NULL);
}

int main(int argc, char *argv[])
{
    FILE *file;
    char line[5];
    char *filename;
    pthread_t ids[1000];
    int i = 0;
    int flag = 0;

    if(argc != 3)
    {
        return -1;
    }

    train = (train_t*) calloc(1, sizeof(train_t));
    train->on_train = 0;
    train->max_passengers = atoi(argv[2]);   // first filename and then max_number //
    train->exit = 0;
    train->pass_exits = 0;
    pass_num = 1;

    filename = argv[1];

    file = fopen(filename, "r");
    if(filename == NULL)
    {
        printf("ERROR: No input file\n");
    }

    train->passengers = (pthread_t*) calloc(train->max_passengers, sizeof(train_t));
    if(train->passengers == NULL)
    {
        printf("ERROR: Calloc Failed\nEXITING...\n");
        return -41;
    }

    train_mon = initMonitor(train_mon);
    pass_ride = initMonitor(pass_ride);
    pass_wait = initMonitor(pass_wait);
    print = initMonitor(print);

    pthread_create(&ids[i], NULL, train_function, NULL);
    printf("Thread Train with id %ld created!\n", ids[i]);
    i++;

    while (fgets(line, sizeof(line) ,file) != NULL)
    {
        if(atoi(line) < 0)
        {
            sleep(abs (atoi(line)) );
            flag = 1;   // last passenger //
            pthread_create(&ids[i], NULL, passengers_function, (void*) (&flag));
            // printf("Thread with id %ld created!\n", ids[i]);  
        }
        else
        {
            sleep(atoi(line));
            pthread_create(&ids[i], NULL, passengers_function, NULL);
            // printf("Thread with id %ld created!\n", ids[i]);
            flag = 0;
        }
    }

    pthread_join(ids[0], NULL);
    
    destroyMonitor(train_mon);
    printf("Monitor train_mon destroyed\n");
    destroyMonitor(pass_ride);
    printf("Monitor pass_ride destroyed\n");
    destroyMonitor(pass_wait);
    printf("Monitor pass_wait destroyed\n");
    destroyMonitor(print);
    printf("Monitor print destroyed\n");

    return 0;
}