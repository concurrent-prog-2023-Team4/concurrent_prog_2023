#include "mysem.h"

int mysem_create(mysem_t *s) 
{
    int id;
    int mtx;
    id = semget(IPC_PRIVATE, 1, 0666);
    mtx = semget(IPC_PRIVATE, 1, 0666);
    if (id == -1 || mtx == -1) 
    {
        perror("semget");
        return -1;
    }
    s->sem_id = id;
    s->sem_id = mtx;
    semctl(s->sem_id, 0, SETVAL, 2);
    return 0;
}

int mysem_init(mysem_t *s, int n) 
{
    if (n != 0 && n != 1)
    {
        printf("Value not 0 or 1.\n");
        return 0;
    }
    if (semctl(s->sem_id, 0, GETVAL) != 2) 
    {
        printf("Semaphore already initialized.\n");
        return -1;
    }
    if (semctl(s->sem_id, 0, SETVAL, n) == -1) 
    {
        perror("semctl");
        return -2;
    }
    if (semctl(s->sem_id, 0, SETVAL, n) == -1) 
    {
        perror("semctl");
        return -3;
    }
    return 1;
}

int mysem_down(mysem_t *s) 
{
    struct sembuf operator;
    int result;

    if (semctl(s->sem_id, 0, GETVAL) == 2) 
    {
        perror("Semaphore not initialized.\n");
        return -1;
    }

    operator.sem_num = 0;
    operator.sem_op = -1;
    operator.sem_flg = 0;

    result = semop(s->sem_id, &operator, 1);
    if(result != 0)
    {
        perror("operation failed!");
        return -2; //operation failed        
    }

    return 1;
}

int mysem_up(mysem_t *s) 
{
    struct sembuf operator;
    int result;
    
    result = semctl(s->sem_id, 0, GETVAL);
    if (result == 2) 
    {
        perror("Semaphore not initialized.\n");
        return -1;
    }
    else if (result == 1)
    {
        perror("Value already on 1.\n");
        return 0;
    }

    operator.sem_num = 0;
    operator.sem_op = 1;
    operator.sem_flg = 0;

    if(semop(s->sem_id, &operator, 1) != 0)
    {
        perror("operation failed!");
        return -2; //operation failed //     
    }

    return 1;
}

int mysem_destroy(mysem_t *s) 
{
    int result;

    if (semctl(s->sem_id, 0, GETVAL) == 2) 
    {
        perror("Semaphore not initialized.\n");
        return -1;
    }

    result = semctl(s->sem_id, 0, IPC_RMID);
    if(result == -1)
    {
        perror("Remove of SEMAPHORE failed!\n");
        return -1;
    }

    result = semctl(s->mtx_id, 0, IPC_RMID);
    if(result == -1)
    {
        perror("Remove of MUTEX failed!\n");
        return -1;
    }

    return 1;
}