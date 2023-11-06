// hw1
#include "hw1.h"
#include <stdio.h>
#include <stdlib.h>

pipe_info pipe_list[5];    // global array of char pointers to the pipe_list 

void pipe_info_init(pipe_info pipe_struct)
{
    pipe_struct.size = 0;
    pipe_struct.read = 0;
    pipe_struct.write = 0;
    pipe_struct.pipe = NULL;
}

int pipe_open (int size)
{
    char *pipe_ptr = NULL;
    int i,j;

    pipe_ptr = (char *) calloc(sizeof(char),size+1);
    if(pipe_ptr == NULL)
        return -2;  // malloc failed
   
    for(i = 0; i < 5; i++)
    {
        if(pipe_list[i].size == 0) //If there's no pipe in that slot, create pipe here
        {
            pipe_list[i].size = size;
            pipe_list[i].pipe = pipe_ptr;
            return i; //global array index
        }
    } 
    return -1;  // no space
}

int pipe_write(int p, char c) 
{   
    if (pipe_list[p].size == 0) {
        return -1;                  //Check if pipe exists
    }
    if (pipe_list[p].write == -1) {
        return -2;                  //Check if pipe's open for writing
    }
    int currWrite = pipe_list[p].write;
    int currSize = pipe_list[p].size;
  
    pipe_list[p].pipe[currWrite] = c;

      if (currWrite + 1 == currSize) {
        pipe_list[p].write = 0;
    } else {
        pipe_list[p].write++;
    }
    return 1;
}

int pipe_writeDone(int p) 
{   
    if (pipe_list[p].size == 0) {
        return -1;                  //Check if pipe exists
    }
    pipe_list[p].write = -1;        //Pipe closed for writing
    return 1;
}

int main()
{
    int i, j, index;

    for(i = 0; i < 5; i++)
    {
        pipe_info_init(pipe_list[i]);
    }

    int newPipe = pipe_open(5);
    for( i = 0; i < 6; i++) {
        pipe_write(newPipe, 'a' + i);
    }
    pipe_writeDone(newPipe); 
    int k = pipe_write(newPipe, '3');
    printf("%d\n", k);
    


    printf("%s", pipe_list[newPipe].pipe);
    printf("\n");

    return 0;
}
