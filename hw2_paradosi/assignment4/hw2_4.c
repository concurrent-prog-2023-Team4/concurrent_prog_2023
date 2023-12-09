#include "hw2_4.h"

void *train_function()
{
    int i;
    int result;
    while(1)
    {   
        
        mysem_down(&train_sem);
        
        mysem_down(&mtx);
        printf(ANSI_COLOR_RED "Train starts!\n" ANSI_COLOR_RESET);
        mysem_up(&mtx);
        
        sleep(5);   // time of ride //

        mysem_down(&mtx);
        printf(ANSI_COLOR_RED "Train stops!\n" ANSI_COLOR_RESET);
        train->on_train = 0;
        if(train->passengers_waiting > train->max_passengers)
            train->passengers_waiting = train->passengers_waiting - train->max_passengers;
        else
            train->passengers_waiting = 0;

        train->waiting_up_needed = train->passengers_waiting;

        for(i = 0; i < train->max_passengers; i++)  // wait for passengers to exit //
        {   
            result = mysem_up(&pass_ride);
            {
                if(result == 0)
                {
                    printf("pass_ride lost up\n");
                }
            }
            pthread_join(train->passengers[i], NULL);
            printf("Thread with id %ld exited... and waiting is %d\n", train->passengers[i], train->passengers_waiting);
            if(train->waiting_up_needed < train->max_passengers && train->waiting_up_needed > 0)
                train->waiting_up_needed--;
        }

        if(train->exit == 1)
        {
            break;
        }
 
        mysem_up(&mtx);
    }
    mysem_up(&mtx);
    pthread_exit(NULL);
}

void *passengers_function(void *varg)
{   
    int result;
    
    mysem_down(&mtx);
    train->passengers_waiting++;
    mysem_up(&mtx);
    int *flag = (int *) varg;
    int inform_exit = 0;
    if(flag != NULL)
    {
        inform_exit = 1;
    }

    while(1)
    {
        mysem_down(&mtx);
        if(train->on_train == train->max_passengers)
        {
           
            printf(ANSI_COLOR_BLUE "Passenger with id %ld waiting and waiting is %d\n" ANSI_COLOR_RESET , pthread_self(), train->passengers_waiting);
            mysem_up(&mtx);
            mysem_down(&pass_wait);
            
        }
        else
        {
            train->on_train++;
            train->passengers[train->on_train-1] = pthread_self();
            printf("Passenger with %ld gets on train\n", pthread_self());
            if(train->max_passengers == train->on_train)
            {
                result = mysem_up(&train_sem);
                if(result == 0)
                {
                    printf("train_sem lost up\n");
                }
            }
            mysem_up(&mtx);
            break;
        }
    }
    mysem_down(&pass_ride); // down because passenger gets on train //
    
    if(train->waiting_up_needed > 0)
    {
        result = mysem_up(&pass_wait);
        printf("pass_wait UP\n");
        if(result == 0)
        {
            printf("pass_waits lost up\n");
        }
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
    int result;

    if(argc != 3)
    {
        return -1;
    }

    train = (train_t*) calloc(1, sizeof(train_t));
    train->on_train = 0;
    train->passengers_waiting = 0;
    train->max_passengers = atoi(argv[2]);   // first filename and then max_number //
    train->exit = 0;
    train->waiting_up_needed = 0;

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

    mysem_create(&pass_ride);
    mysem_create(&pass_wait);
    mysem_create(&mtx);
    mysem_create(&train_sem);

    mysem_init(&mtx, 1);
    mysem_init(&train_sem, 0);
    mysem_init(&pass_wait, 0);
    mysem_init(&pass_ride, 0);

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
            printf("Thread with id %ld created!\n", ids[i]);  
        }
        else
        {
            sleep(atoi(line));
            pthread_create(&ids[i], NULL, passengers_function, NULL);
            printf("Thread with id %ld created!\n", ids[i]);
            flag = 0;
        }
    }

    pthread_join(ids[0], NULL);
    
    result = mysem_destroy(&pass_ride);
    if(result == 1)
        printf("Semaphore pass_ride destroyed succesfully!\n");
    result = mysem_destroy(&pass_wait);
    if(result == 1)
        printf("Semaphore pass_ride destroyed succesfully!\n");
    result = mysem_destroy(&mtx);
    if(result == 1)
        printf("Semaphore pass_ride destroyed succesfully!\n");
    result = mysem_destroy(&train_sem);
    if(result == 1)
        printf("Semaphore pass_ride destroyed succesfully!\n");

}