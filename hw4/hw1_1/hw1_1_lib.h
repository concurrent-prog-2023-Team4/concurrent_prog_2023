// Header file for hw1.1 //
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct pipe_info 
{
    int size;
    int write;
    int read;
    char *pipe;
} pipe_info;

pipe_info pipe_list[5];    // global array of char pointers to the pipe_list //

void pipe_info_init(pipe_info pipe_struct);

int pipe_open(int size);

int pipe_write(int p, char c);

int pipe_writeDone(int p);

int pipe_read(int p, char *c);

int pipe_is_full(int p);
