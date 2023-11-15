// working Version
#include "hw1_1.h"
#define MAX_TEXT 6000

char text[4000];
int exit_main;
int finish;
int start_write;

void *mythread(void *varg)
{
    int *size = (int *)varg;
    pipe_open(*size);
    printf("Size is %d\n", pipe_list[0].size);

    return NULL;
}

void *thread_func_1(void *argv)
{
    int i = 0;
    char out[4000];
    int flag = -2;

    while(1)
    {
        if(start_write == 1)
            break;
    }

    for(i = 0; text[i] != '\0'; i++)
    {
        while(1)
        {
            if(pipe_list[0].pipe[pipe_list[0].write] == '\0')
                break;
        }
        pipe_write(0, text[i]);      // write a char to pipe_1 //
    }

    pipe_writeDone(0);
////////////////////////////

    while(1)
    {
        if(finish == 1)
        {
            break;
        }
    }

    while(1)
    {
        flag = pipe_read(1, out);
        if(flag == 0)
        {
            break;
        }
    }
    printf("The out is %s\n", out);
    FILE *copy2;
    
    copy2 = fopen(".copy2", "w");

    if(copy2 != NULL)
    {
        fprintf(copy2, "%s", out);
    }

    exit_main = 1;

    pthread_exit(NULL);

}

void *thread_func_2(void *argv)
{
    int i = 0;
    char out[4000];
    int flag = -2;
    FILE *copy;
    char buffer[4000] = {'\0'};

    while(1)
    {
        flag = pipe_read(0, out);
        if(flag == 0)
        {
            break;
        }
    }
    printf("The out is : %s\n", out);
    
    copy = fopen(".copy", "w+");     // if open it with w, if exists make it empty else create

    if(copy != NULL)
    {
        fprintf(copy, "%s", out);
    }

    finish = 1;     // send command that write completed //
////////////////////////

    fseek(copy, 0, SEEK_SET);
    fread(buffer, 1, 4000, copy);

    printf("buffer is %s\n", buffer);

    for(i = 0; buffer[i] != '\0'; i++)
    {
        while(1)
        {
            if(pipe_list[1].pipe[pipe_list[1].write] == '\0')
                break;
        }
        pipe_write(1, buffer[i]);      // write a char to pipe_1 //
    }
    pipe_writeDone(1);
    
    pthread_exit(NULL);
}

int main()
{
    pthread_t p1, p2;

    int ret_1, ret_2;
    int size = 64;

    char line[MAX_TEXT] = {'\0'};

    const char *filename = ".copy";
    const char *filename2 = ".copy2";


    // ret = pthread_create(&p2, NULL, mythread, NULL);
    if(access(filename, F_OK) != -1)
    {
        remove(filename);
    }

    if(access(filename2, F_OK) != -1)
    {
        remove(filename2);
    }

    pipe_open(size);     // open pipes
    pipe_open(size);

    finish = 0;
    exit_main = 0;

    // fgets(text, sizeof(text), stdin);

    while(fgets(line, sizeof(line), stdin) != NULL)
    {
        strncat(text, line, sizeof(text));
    }

    // while( scanf("%s ", text) != EOF);
    printf("The input is %s\n", text);
    
    ret_1 = pthread_create(&p1, NULL, thread_func_1, NULL);
    if(ret_1 == 0)
        printf("Thread created succesfully\n");

    ret_2 = pthread_create(&p2, NULL, thread_func_2, NULL);
    if(ret_2 == 0)
        printf("Thread created succesfully\n");

    start_write = 1;

    while(1)
    {
        if(exit_main == 1)
        {
            break;
        }
    }


    return 0;
}