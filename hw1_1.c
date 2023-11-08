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


    //char text[128];
    char out[128];

    // FILE *kati;
    // kati = fopen("copy", "w+");

    // FILE *kati2;
    // kati2 = fopen("copy2", "w+");
    printf("currpipe is %d\n", *curPipe);

    if(*curPipe == 0)
    {
        //scanf(" %100s", text);
        FILE *kati2;
        kati2 = fopen("copy2", "w+");

        printf("Text is %s\n", text);
        for (int i = 0; text[i] != '\0'; i++)
            pipe_write(0, text[i]);
        pipe_writeDone(0);

        for(int i = 0; i < 128; i++)
        {
            pipe_read(1, out);
        }
        printf("out is %s\n", out);

        fseek(kati2, 0, SEEK_END);      // writes copy2
        // fseek(kati2, 0);
        long filesize = ftell(kati2);

        // fread(out, filesize, 1, kati);
        fgets(out, filesize, kati2);
        fprintf(kati2, out);
    }


    
    else if (*curPipe == 1)
    {
        FILE *kati;
        kati = fopen("copy", "w+");

        for(int i = 0; i < 128; i++)
        {
            pipe_read(0, out);
        }
        fprintf(kati, out);

        fseek(kati, 0, SEEK_END);
        long filesize = ftell(kati);

        // fread(out, filesize, 1, kati);
        fgets(out, filesize, kati);
        printf("Out is %s\n", out);
        
        for (int i = 0; out[i] != '\0'; i++)
            pipe_write(1, out[i]);
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

    scanf(" %s", text);
    ret_1 = pthread_create(&p1, NULL, thread_1, (void*) &arg1);
    if(ret_1 == 0)
        printf("Thread created succesfully\n");

    ret_2 = pthread_create(&p2, NULL, thread_1, (void*) &arg2);
    if(ret_2 == 0)
        printf("Thread created succesfully\n");


    return 0;
}
