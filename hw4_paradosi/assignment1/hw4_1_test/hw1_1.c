// hw1_1 c file //
#include "hw1_1.h"
#define MAX_TEXT 65

int exit_main;      // variable that thread informs main to finish //
int finish;         // variable that threads use to synchronize //
int start_write;    // main informs thread to start writing //

void *reader(void *argv)
{
    int i = 0;
    int j = 0;
    char ch;
    FILE *filename = NULL;

    while(1)
    {
        if(start_write == 1)
            break;
    }
    printf("Reader started\n");

    filename = fopen(input, "rb");

    while((ch = fgetc(filename)) != EOF)
    {
        pipe_write(0, ch);      // write a char to pipe_1 //
        i++;
        if(i == pipe_list[0].size)
        {
            i = 0;
            printf(ANSI_COLOR_BLUE "Reader start reading\n" ANSI_COLOR_RESET);
            printf("Reader switched to Writer\n");
            reader_cot->next_cot = writer_cot->cot;
            mycoroutines_switchto(reader_cot);
            j = j + pipe_list[0].size;
        }
    }
    printf("Reader switched to Writer\n");
    reader_cot->next_cot = writer_cot->cot;
    mycoroutines_switchto(reader_cot);
    j = j + pipe_list[0].size;

    pipe_writeDone(0);
    reader_cot->next_cot = writer_cot->cot;
    mycoroutines_switchto(reader_cot);

    return NULL;
}

void *writer(void *argv)
{
    int i = 0;
    char out[MAX_TEXT];
    int pipe_read_result = -2;
    FILE *copy;
    int flag = 0;
    char c;

    printf("Writer started\n");
    copy = fopen("out.copy", "w+b");     // if open it with a+, if exists append to it else create //
    while(1)
    {
        if(pipe_list[0].valid_size == 0)
        {
            break;
        }
        for(i = 0; i < pipe_list[0].valid_size; i++)
        {
            pipe_read_result = pipe_read(0, &c);
            if(copy != NULL)
            {
                fprintf(copy, "%c", c);
            }
            else
            {
                printf("Error opening file\n");
            }
            
            if(pipe_read_result == 0)
            {
                flag = 1;
                break;
            }
        }
        pipe_list[0].valid_size = 0;
        if(flag == 1)
        {
            break;
        }

        printf(ANSI_COLOR_RED "Writer started writing to file\n" ANSI_COLOR_RESET);
        if(copy != NULL)
        {
            fprintf(copy, "%s", out);
        }
        else
        {
            printf("Error opening file\n");
        }
        out[0] = '\0';
        printf("Writer switched to Reader\n");
        writer_cot->next_cot = reader_cot->cot;
        mycoroutines_switchto(writer_cot);
    }

    finish = 1;     // send command that write completed //

    writer_cot->next_cot = main_co->cot;
    writer_cot->cot->uc_link = main_co->cot;
    printf("Writer switched to main\n");
    mycoroutines_switchto(writer_cot);

    return NULL;
}

int main(int argc, char *argv[])
{
    int size = 64;  // size of each pipe //

    const char *filename = ".copy";

    if(access(filename, F_OK) != -1)    // if file .copy or .copy2 exists, delete it //
    {
        remove(filename);
    }

    if(argc != 2)
    {
        printf("Usage: ./hw1_1 <input_file>\n");
        return -1;
    }

    pipe_open(size);     // open pipes //
    pipe_open(size);

    finish = 0;
    exit_main = 0;

    main_co = (co_t *) malloc(sizeof(co_t));
    reader_cot = (co_t *) malloc(sizeof(co_t));
    writer_cot = (co_t *) malloc(sizeof(co_t));

    main_co->cot = (ucontext_t *) malloc(sizeof(ucontext_t));
    reader_cot->cot = (ucontext_t *) malloc(sizeof(ucontext_t));
    writer_cot->cot = (ucontext_t *) malloc(sizeof(ucontext_t));

    mycoroutines_create(main_co, (void *) main, NULL);
    mycoroutines_create(reader_cot, (void *) reader, NULL);
    mycoroutines_create(writer_cot, (void *) writer, NULL);

    main_co->cot->uc_link = reader_cot->cot;
    reader_cot->cot->uc_link = writer_cot->cot;
    writer_cot->cot->uc_link = reader_cot->cot;
    

    start_write = 1;
    input = (char *) malloc(sizeof(char) * strlen(argv[1]) + 1);

    strcpy(input, argv[1]);
    
    main_co->next_cot = reader_cot->cot;
    mycoroutines_switchto(main_co);

    printf("Main exits\n");

    return 0;
}