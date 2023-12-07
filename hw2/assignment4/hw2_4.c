#include "hw2_4.h"
/* 1) destroy se ola 
   2) exoume mia xameni up otan kanoun ola ta threads waiting++
   3) den termatizei sosta
   4) an den kanoun ola ta threads waiting++ tote den jipname ola 
      ta threads otan termatizei ena allo*/

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
        // mysem_up(&mtx);
        train->on_train = 0;
        if(train->passengers_waiting > train->max_passengers)
            train->passengers_waiting = train->passengers_waiting - train->max_passengers;
        else
            train->passengers_waiting = 0;

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
        }
        // train->on_train = 0;
        // mysem_down(&mtx);
        // mysem_up(&mtx);
        if(train->exit == 1)
        {
            break;
        }
        // else
        // {
        //     for(i = 0; i < train->passengers_waiting; i++)
        //     {
        //         mysem_up(&pass_wait);
        //     }
        // }
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
            // den mpainei //
            
            // train->passengers_waiting++;
            printf(ANSI_COLOR_BLUE "Passenger waiting and waiting is %d\n" ANSI_COLOR_RESET , train->passengers_waiting);
            mysem_up(&mtx);
            mysem_down(&pass_wait);
            // if(train->passengers_waiting > 0)           // passenger wakes and checks if others are waiting
            // {
            //     train->passengers_waiting--;            // if other passenger waiting 
            //     result = mysem_up(&pass_wait);                   // wake him up
            //     if (result == 0 ) {
            //         printf("pass_wait lost UP\n");
            //     }
            // }
            //else
            //{
                //mysem_up(&mtx);                         
            //}
            
        }
        else
        {
            train->on_train++;
            // train->passengers_waiting--;
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

    // mysem_down(&mtx);
    // train->on_train--;
    if(train->passengers_waiting > 0)
    {
        // train->passengers_waiting--;
        result = mysem_up(&pass_wait);
        if(result == 0)
        {
            printf("pass_waits lost up\n");
        }
    }
    //mysem_down(&mtx);
    if(inform_exit)
    {
        train->exit = 1;
    }
    //mysem_up(&mtx);

    pthread_exit(NULL);
}

int main(int argc, char *argv[])
{
    FILE *file;
    char line[5];
    char *filename;
    pthread_t ids[100];
    int i = 0;
    int flag = 0;

    if(argc != 3)
    {
        // printf("ERROR: Incorrect arguments\n");
        return -1;
    }

    train = (train_t*) calloc(1, sizeof(train_t));
    train->on_train = 0;
    train->passengers_waiting = 0;
    train->max_passengers = atoi(argv[2]);   // first filename and then max_number //
    train->exit = 0;

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
            flag = 1;   //last passenger
            pthread_create(&ids[i], NULL, passengers_function, (void*) (&flag));
            printf("Thread with id %ld created!\n", ids[i]);  
        }
        sleep(atoi(line));
        pthread_create(&ids[i], NULL, passengers_function, NULL);
        printf("Thread with id %ld created!\n", ids[i]);
    }

    pthread_join(ids[0], NULL);

}