// hw1_1 library //
#include "hw1_1_lib.h"

void pipe_info_init(pipe_info pipe_struct)
{
    pipe_struct.size = 0;
    pipe_struct.read = 0;
    pipe_struct.write = 0;
    pipe_struct.pipe = NULL;
    pipe_struct.valid_size = 0;
}

int pipe_open (int size)
{
    char *pipe_ptr = NULL;
    int i;

    pipe_ptr = (char *) calloc(size + 1, sizeof(char));
    if(pipe_ptr == NULL)
        return -2;  // malloc failed //
 
    for(i = 0; i < 5; i++)
    {
        if(pipe_list[i].size == 0) // If there's no pipe in that slot, create pipe here //
        {
            pipe_list[i].size = size;
            pipe_list[i].pipe = pipe_ptr;
            return i;       // global array index //
        }
    } 
    
    return -1;  // no space //
}

int pipe_write(int p, char c) 
{   
    if (pipe_list[p].size == 0) 
    {
        return -1;                  // Check if pipe exists // 
    }
    if (pipe_list[p].write == -1) 
    {
        return 0;                  // Check if pipe's open for writing //
    }
    int currWrite = pipe_list[p].write;
    int currSize = pipe_list[p].size;

    pipe_list[p].pipe[currWrite] = c;
    pipe_list[p].valid_size++;

    if (currWrite + 1 == currSize) 
    {
        pipe_list[p].write = 0;
    } 
    else 
    {
        pipe_list[p].write++;
    }
    
    return 1;
}

int pipe_writeDone(int p) 
{   
    if (pipe_list[p].size == 0) 
    {
        return -1;                  // Check if pipe exists //
    }
    pipe_list[p].write = -1;        // Pipe closed for writing //

    return 1;
}

int pipe_read(int p, char *c)
{   
    int currRead = pipe_list[p].read;

    *c = pipe_list[p].pipe[currRead];
    
    pipe_list[p].pipe[currRead] = '\0';        // when it reads a char, make slot \0 //

    if(pipe_list[p].read + 1 == pipe_list[p].size)
    {
        pipe_list[p].read = 0;
    }
    else 
    {
        pipe_list[p].read++;
    }

    if(pipe_list[p].write == -1)
    {
        return 0;
    }
    
    return 1;
}