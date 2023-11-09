// pthreads brokos
#include "hw1_1.h"

char text[128];

// void *mythread(void *varg)
// {
//     int *size = (int *)varg;
//     pipe_open(*size);
//     printf("Size is %d\n", pipe_list[0].size);

//     return NULL;
// }

// void *thread_1(void *varg)
// {     
//     int *curPipe = (int *) varg;



//     //char text[128];
//     //char out_0[128], out_1[128];

//     char *out_0;
//     char *out_1;

//     // FILE *copy;
//     // copy = fopen("copy", "w+");

//     // FILE *copy2;
//     // copy2 = fopen("copy2", "w+");
//     printf("currpipe is %d\n", *curPipe);

//     if(*curPipe == 0)
//     {   // thread 0

//         //scanf(" %100s", text);
//         int i = 0;
//         FILE *copy2;
//         copy2 = fopen("copy2", "w+");

//         out_0 = (char*) malloc(129*sizeof(char));

//         //printf("Text is %s\n", text);
//         for (int i = 0; text[i] != '\0'; i++)
//             pipe_write(0, text[i]);
//         pipe_writeDone(0);

//         // for(int i = 0; i < 128; i++)
//         // {
//         //     pipe_read(1, out_0);
//         // }
//         do
//         {
//             pipe_read(1, out_0);
//             i++;
//         } while (out_0[i] != '\0');
        
//         printf("out is %s\n", out_0);

//         fseek(copy2, 0, SEEK_END);      // writes copy2
//         // fseek(copy2, 0);
//         long filesize = ftell(copy2);

//         // fread(out, filesize, 1, copy);
//         fseek(copy2, 0, SEEK_SET);
//         fgets(out_0, filesize, copy2);
//         fprintf(copy2, "%s", out_0);
//     }


    
//     else if (*curPipe == 1)
//     {
//         FILE *copy;
//         copy = fopen("copy", "w+");
//         int i = 0;
//         char temp[128] = {'\0'};

//         out_1 = (char*) malloc(129*sizeof(char));

//         // for(int i = 0; i < 128; i++)
//         // {
//         //     pipe_read(0, out_1);
//         // }
//         do
//         {
//             pipe_read(0, out_1);
//             i++;
//         } while (out_1[i] != '\0');

//         fprintf(copy, "%s", out_1);       // write to copy_1

//         fseek(copy, 0, SEEK_END);
//         long filesize = ftell(copy);

//         // fread(out, filesize, 1, copy);
//         fseek(copy, 0, SEEK_SET);
        
//         fgets(temp, filesize, copy);
//         printf("Out is %s\n", temp);
        
//         for (int i = 0; temp[i] != '\0'; i++)
//             pipe_write(1, temp[i]);
//         pipe_writeDone(1);

//     }
//     else
//     {
//         printf("Nothing\n");
//     }

//     return NULL;
// }

// int main()
// {
//     pthread_t p1, p2;

//     int ret_1, ret_2;
//     int arg1 = 0;
//     int arg2 = 1;
//     int pipe_id1, pipe_id2;
//     int size = 64;

//     // linear

//     int i = 0, j = 0;
//     char out_1[128] = {'\0'};
//     pipe_id1 = pipe_open(size);     // open pipes
//     pipe_id2 = pipe_open(size);

//     FILE *copy;
//     copy = fopen("copy", "w+");

//     //scanf(" %s", text);     // input

//     // if (fgets(text, sizeof(text), stdin) != NULL) 
//     // {
//     //     // Remove the newline character if present
//     //     size_t len = strlen(text);
//     //     if (len > 0 && text[len - 1] == '\n') {
//     //         text[len - 1] = '\0';
//     //     }
//     // }

//     fgets(text, sizeof(text), stdin);

//     for(i = 0; text[i] != '\0'; i++)
//     {
//         pipe_write(0, text[i]);
//     }
//     printf("the pipe is %s\n", pipe_list[0].pipe);

//     int len = strlen(text);

//     // do
//     // {
//     //     pipe_read(0, out_1);
//     //     i++;
//     // } while (out_1[i-1] != '\0');

//     for(j = 0; j < len; j++)
//     {
//         pipe_read(0, out_1);
//     }

//     printf("Out is %s\n", out_1);

//     fwrite(out_1, sizeof(out_1[0]), len, copy);

//     char input_2[128] = {'\0'};

//     fseek(copy, 0, SEEK_SET);
//     fread(input_2, len, sizeof(char), copy);

//     printf("fread result is %s\n", input_2);

//     for(i = 0; i < len; i++)
//     {
//         pipe_write(1, input_2[i]);
//     }
//     printf("the pipe 2 is %s\n", pipe_list[1].pipe);

//     for( i = 0; i < len; i++)
//     {
//         pipe_read(1, out_1);
//     }

//     FILE *copy2;
//     copy2 = fopen("copy2", "w+");
//     fwrite(out_1, sizeof(out_1[0]), len, copy2);



//     return 0;
// }
