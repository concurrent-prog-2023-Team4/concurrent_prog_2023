// pthreads brokos
#include "hw1_1.h"

char text[128];

void *mythread(void *varg)
{
    int *size = (int *)varg;
    pipe_open(*size);
    printf("Size is %d\n", pipe_list[0].size);

    return NULL;
}

void *thread_1(void *varg)
{     
    int *curPipe = (int *) varg;

    char *out_0;
    char *out_1;


    printf("currpipe is %d\n", *curPipe);

    if(*curPipe == 0)
    {   // thread 0

        int i = 0, j = 0;
        FILE *copy2;
        //copy2 = fopen("copy2", "w+");

        out_0 = (char*) malloc(129*sizeof(char));


        for (int i = 0; text[i] != '\0'; i++)
            pipe_write(0, text[i]);
        pipe_writeDone(0);

        int len = strlen(text);

        for(j = 0; j < len; j++)
        {
            pipe_read(1, out_0);
        }

        copy2 = fopen("copy2", "w+");
        fwrite(out_0, sizeof(out_0[0]), len, copy2);
    }


    
    else if (*curPipe == 1)
    {
        FILE *copy;
        copy = fopen("copy", "w+");
        int i = 0, j = 0;
        char temp[128] = {'\0'};

        char input_2[128] = {'\0'};

        out_1 = (char*) malloc(129*sizeof(char));

        int len = strlen(text);

        for(j = 0; j < len; j++)
        {
            pipe_read(0, out_1);
        }

        fwrite(out_1, sizeof(out_1[0]), len, copy);


        fseek(copy, 0, SEEK_SET);
        fread(input_2, len, sizeof(char), copy);
        
        for(i = 0; i < len; i++)
        {
            pipe_write(1, input_2[i]);
        }
        pipe_writeDone(1);

    }
    else
    {
        printf("Nothing\n");
    }

    return NULL;
}

int main()
{
    pthread_t p1, p2;

    int ret_1, ret_2;
    int arg1 = 0;
    int arg2 = 1;
    int pipe_id1, pipe_id2;
    int size = 64;


    // ret = pthread_create(&p2, NULL, mythread, NULL);

    pipe_id1 = pipe_open(size);     // open pipes
    pipe_id2 = pipe_open(size);

    fgets(text, sizeof(text), stdin);
    ret_1 = pthread_create(&p1, NULL, thread_1, (void*) &arg1);
    if(ret_1 == 0)
        printf("Thread created succesfully\n");

    ret_2 = pthread_create(&p2, NULL, thread_1, (void*) &arg2);
    if(ret_2 == 0)
        printf("Thread created succesfully\n");


    return 0;
}
