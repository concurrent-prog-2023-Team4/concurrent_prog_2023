// working Version
#include "hw1_1.h"

char text[128];
int exit_main;
int finish;

void *mythread(void *varg)
{
    int *size = (int *)varg;
    pipe_open(*size);
    printf("Size is %d\n", pipe_list[0].size);

    return NULL;
}

// void *thread_1(void *varg)
// {     
//     int *curPipe = (int *) varg;

//     char *out_0;
//     char *out_1;


//     printf("currpipe is %d\n", *curPipe);

//     if(*curPipe == 0)
//     {   // thread 0

//         int i = 0, j = 0;
//         FILE *copy2;
//         //copy2 = fopen("copy2", "w+");

//         out_0 = (char*) malloc(129*sizeof(char));


//         for (int i = 0; text[i] != '\0'; i++)
//             pipe_write(0, text[i]);
//         pipe_writeDone(0);

//         int len = strlen(text);

//         for(j = 0; j < len; j++)
//         {
//             pipe_read(1, out_0);
//         }

//         copy2 = fopen("copy2", "w+");
//         fwrite(out_0, sizeof(out_0[0]), len, copy2);
//     }
    
//     else if (*curPipe == 1)
//     {
//         FILE *copy;
//         copy = fopen("copy", "w+");
//         int i = 0, j = 0;
//         char temp[128] = {'\0'};

//         char input_2[128] = {'\0'};

//         out_1 = (char*) malloc(129*sizeof(char));

//         int len = strlen(text);

//         for(j = 0; j < len; j++)
//         {
//             pipe_read(0, out_1);
//         }

//         fwrite(out_1, sizeof(out_1[0]), len, copy);


//         fseek(copy, 0, SEEK_SET);
//         fread(input_2, len, sizeof(char), copy);
        
//         for(i = 0; i < len; i++)
//         {
//             pipe_write(1, input_2[i]);
//         }
//         pipe_writeDone(1);

//     }
//     else
//     {
//         printf("Nothing\n");
//     }

//     return NULL;
// }
int start_write;

void *thread_func_1(void *argv)
{
    int i = 0;
    char out[128];
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
        fprintf(copy2, out);
    }

    exit_main = 1;

}

void *thread_func_2(void *argv)
{
    int i = 0;
    char out[128];
    int flag = -2;
    FILE *copy;
    char buffer[128] = {'\0'};

    while(1)
    {
        flag = pipe_read(0, out);
        if(flag == 0)
        {
            break;
        }
    }
    printf("The out is %s\n", out);
    
    copy = fopen(".copy", "rb");

    if(copy != NULL)
    {
        fprintf(copy, out);
    }

    finish = 1;     // send command that write completed //
////////////////////////

    fseek(copy, 0, SEEK_SET);
    fread(buffer, 1, 127, copy);

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
}

int main()
{
    pthread_t p1, p2;

    int ret_1, ret_2;
    int pipe_id1, pipe_id2;
    int size = 64;


    // ret = pthread_create(&p2, NULL, mythread, NULL);

    pipe_id1 = pipe_open(size);     // open pipes
    pipe_id2 = pipe_open(size);

    finish = 0;
    exit_main = 0;

    fgets(text, sizeof(text), stdin);

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

//testtt
