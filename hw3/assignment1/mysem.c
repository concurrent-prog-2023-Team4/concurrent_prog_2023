#include "mysem.h"

int mysem_create(mysem_t *s) 
{
    int id;
    id = rand();
    if (id == -1) 
    {
        perror("semget");
        return -1;
    } 
    s->sem_id = id;
    // s->mutex = PTHREAD_MUTEX_INITIALIZER;
    // s->cond = PTHREAD_COND_INITIALIZER;
    pthread_mutex_init(&(s->mutex), NULL);
    //pthread_mutex_init(&(s->safe_read), NULL);
    pthread_cond_init(&(s->cond), NULL);
    s->value = 2;

    return 0;
}

int mysem_init(mysem_t *s, int n) 
{
    if (n != 0 && n != 1)
    {
        printf("Value not 0 or 1.\n");
        return 0;
    }
    if (s->value != 2) 
    {
        printf("Semaphore already initialized.\n");
        return -1;
    }
    s->value = n;
    
    return 1;
}

int mysem_down(mysem_t *s) 
{
    

    // pthread_mutex_lock(&(s->safe_read));
    pthread_mutex_lock(&(s->mutex));
    if(s->value <= 0)
    {
        s->value--;
        // pthread_mutex_unlock(&(s->safe_read));
        pthread_cond_wait(&(s->cond), &(s->mutex));
    }
    else if(s->value == 1)
    {
        s->value--;
        // pthread_mutex_unlock(&(s->safe_read));
    }
    pthread_mutex_unlock(&(s->mutex));
    
    return 1;
}

int mysem_up(mysem_t *s) 
{

    int result;
    
    // pthread_mutex_lock(&(s->safe_read));
    pthread_mutex_lock(&(s->mutex));
    result = s->value;
    if (result == 2) 
    {
        perror("Semaphore not initialized.\n");
        //pthread_mutex_unlock(&(s->safe_read));
        pthread_mutex_unlock(&(s->mutex));
        return -1;
    }
    else if (result == 1)
    {
        perror("Value already on 1.\n");
        //pthread_mutex_unlock(&(s->safe_read));
        pthread_mutex_unlock(&(s->mutex));
        return 0;
    }
    else if (result == 0)
    {   
        s->value++;
        pthread_mutex_unlock(&(s->mutex));
    }
    else 
    {
        pthread_cond_signal(&(s->cond));
        pthread_mutex_unlock(&(s->mutex));
        s->value++;
    }
   // pthread_mutex_unlock(&(s->safe_read));

    return 1;
}

int mysem_destroy(mysem_t *s) 
{
    int result;

    result = s->value;

    if (result == 2) 
    {
        perror("Semaphore not initialized.\n");
        return -1;
    }

    pthread_mutex_destroy(&(s->mutex));
    pthread_cond_destroy(&(s->cond));

    return 1;
}