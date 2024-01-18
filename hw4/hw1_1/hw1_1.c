// hw1_1 c file //
#include "hw1_1.h"
#define MAX_TEXT 6000   // max number of char that test can contain //

char text[MAX_TEXT];    // input text from file //
int exit_main;      // variable that thread informs main to finish //
int finish;         // variable that threads use to synchronize //
int start_write;    // main informs thread to start writing //

void *thread_func_1(void *argv)
{
    int i = 0;
    int j = 0;
    char out[MAX_TEXT];
    int pipe_read_result = -2;
    char ch;
    FILE *filename = NULL;

    while(1)
    {
        if(start_write == 1)
            break;
    }
    printf("Thread 1 started\n");

    filename = fopen(input, "r");

    //while(text[j] != '\0')
    while( (ch = fgetc(filename)) != EOF)
    {
        //for(i = 0; i < pipe_list[0].size; i++)
       // {
            pipe_write(0, ch);      // write a char to pipe_1 //
        //}
        i++;
        if(i == pipe_list[0].size)
        {
            i = 0;
            printf("Thread 1 switched to thread 2\n");
            mycoroutines_switchto(thread_1, thread_2);
            j = j + pipe_list[0].size;
        }
    }
    printf("Thread 1 switched to thread 2\n");
    mycoroutines_switchto(thread_1, thread_2);
    j = j + pipe_list[0].size;

    pipe_writeDone(0);
    mycoroutines_switchto(thread_1, thread_2);
}

void *thread_func_2(void *argv)
{
    int i = 0;
    char out[MAX_TEXT];
    int pipe_read_result = -2;
    FILE *copy;
    char buffer[MAX_TEXT] = {'\0'};
    int flag = 0;

    printf("Thread 2 started\n");
    // while(1)
    copy = fopen(".copy", "w+");     // if open it with a+, if exists append to it else create //
    while(1)
    {
        for(i = 0; i < pipe_list[0].size; i++)
        {
            pipe_read_result = pipe_read(0, out);
            printf("pipe_read_result = %d\n", pipe_read_result);
            if(pipe_read_result == 0)
            {
                printf("pipe_read_result == 0\n");
                flag = 1;
                break;
            }
        }
        if(flag == 1)
        {
            break;
        }
        printf("Thread 2 switched to thread 1\n");

        printf("Thread 2 started writing to file\n");
        if(copy != NULL)
        {
            fprintf(copy, "%s", out);
        }
        else
        {
            printf("Error opening file\n");
        }
        out[0] = '\0';
        mycoroutines_switchto(thread_2, thread_1);
    }

    finish = 1;     // send command that write completed //

    thread_2->cot->uc_link = main_co->cot;
    printf("Thread 2 switched to main\n");
    mycoroutines_switchto(thread_2, main_co);
    // fseek(copy, 0, SEEK_SET);
    // fread(buffer, 1, MAX_TEXT, copy);
}

int main(int argc, char *argv[])
{
    pthread_t p1, p2;

    int ret_1, ret_2;
    int size = 64;  // size of each pipe //

    char line[MAX_TEXT] = {'\0'};

    const char *filename = ".copy";
    // const char *filename2 = ".copy2";

    if(access(filename, F_OK) != -1)    // if file .copy or .copy2 exists, delete it //
    {
        remove(filename);
    }

    if(argc != 2)
    {
        printf("Usage: ./hw1_1 <input_file>\n");
        return -1;
    }

    // if(access(filename2, F_OK) != -1)
    // {
    //     remove(filename2);
    // }

    pipe_open(size);     // open pipes //
    pipe_open(size);

    finish = 0;
    exit_main = 0;


    // while(fgets(line, sizeof(line), stdin) != NULL)
    // {
    //     strncat(text, line, sizeof(text));
    // }

    main_co = (co_t *) malloc(sizeof(co_t));
    thread_1 = (co_t *) malloc(sizeof(co_t));
    thread_2 = (co_t *) malloc(sizeof(co_t));

    main_co->cot = (ucontext_t *) malloc(sizeof(ucontext_t));
    thread_1->cot = (ucontext_t *) malloc(sizeof(ucontext_t));
    thread_2->cot = (ucontext_t *) malloc(sizeof(ucontext_t));

    // mycoroutines_init(main) //
    mycoroutines_create(main_co, main, NULL);
    mycoroutines_create(thread_1, thread_func_1, NULL);
    mycoroutines_create(thread_2, thread_func_2, NULL);

    main_co->cot->uc_link = thread_1->cot;
    thread_1->cot->uc_link = thread_2->cot;
    thread_2->cot->uc_link = thread_1->cot;
    

    start_write = 1;
    input = (char *) malloc(sizeof(char) * strlen(argv[1]) + 1);

    strcpy(input, argv[1]);
    
    mycoroutines_switchto(main_co, thread_1);

    printf("Main exits\n");

    return 0;
}