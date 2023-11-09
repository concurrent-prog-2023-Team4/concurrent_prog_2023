// hw1
#include "hw1_1_lib.h"

#define DEBUG

// pipe_info pipe_list[5];    // global array of char pointers to the pipe_list 

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
    int i;

    pipe_ptr = (char *) calloc(size + 1, sizeof(char));   // oso kanoume print +1 allios size
    if(pipe_ptr == NULL)
        return -2;  // malloc failed

    // for(i = 0; i < size; i++)
    // {
    //     pipe_ptr[i] = 'DLE';        // I think it is NULL -- non-printable char
    // }
   
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
    if (pipe_list[p].size == 0) 
    {
        return -1;                  //Check if pipe exists
    }
    if (pipe_list[p].write == -1) 
    {
        return 0;                  //Check if pipe's open for writing
    }
    int currWrite = pipe_list[p].write;
    int currSize = pipe_list[p].size;

    //while(pipe_list[p].pipe[currWrite] != '\0');
    
    pipe_list[p].pipe[currWrite] = c;

    if (currWrite + 1 == currSize) 
    {
        pipe_list[p].write = 0;
    } else {
        pipe_list[p].write++;
    }
    return 1;
}

int pipe_writeDone(int p) 
{   
    if (pipe_list[p].size == 0) 
    {
        return -1;                  //Check if pipe exists
    }
    pipe_list[p].write = -1;        //Pipe closed for writing
    return 1;
}

int pipe_read(int p, char *c)   //  it is string
{   
    int currRead = pipe_list[p].read;
    int counter = 0;

    
    if(pipe_list[p].write == -1)        // only if pipe is closed for write
    {
        for(int i = 0; i < pipe_list[p].size; i++)
        {
            if (pipe_list[p].pipe[i] == '\0') 
            {
                counter++ ;    
            }
        }
        if (counter == pipe_list[p].size)
        {
            pipe_list[p].read = -1;
            return 0;   // if pipe is empty and write is close  
        }
    }
    
    // if( (pipe_list[p].read == -1) && (pipe_list[p].write == -1) )
    // {
    //     return 0;   // if pipe is empty and write is close  
    // }

    // c = pipe_list[p].pipe[currRead];

    while(pipe_list[p].pipe[currRead] == '\0');

    c = strncat(c, &pipe_list[p].pipe[currRead], 1);
    pipe_list[p].pipe[currRead] = '\0';
    pipe_list[p].read++;
    
    return 1;
}


// int main()
// {
//     int i;
//     char *text = NULL;

//     for(i = 0; i < 5; i++)
//     {
//         pipe_info_init(pipe_list[i]);
//     }

//     int newPipe = pipe_open(5);
//     for( i = 0; i < 6; i++) 
//     {
//         pipe_write(newPipe, 'a' + i);
//     }
//     pipe_writeDone(newPipe);

//     text = (char*) calloc((pipe_list[newPipe].size+1), sizeof(char) );      // it has to be calloc because it adds \0

//     int k = pipe_write(newPipe, '3');
//     printf("%d\n", k);
//     printf("%s", pipe_list[newPipe].pipe);
//     printf("\n");

//     pipe_read(newPipe, text);
//     pipe_read(newPipe, text);
//     pipe_read(newPipe, text);
//     pipe_read(newPipe, text);
//     pipe_read(newPipe, text);
//     pipe_read(newPipe, text);

//     printf("The text is %s\n", text);

    
    
//     #ifdef DEBUG
//     for(i = 0; i <  pipe_list[newPipe].size; i++)
//     {
//         printf("%c\n", pipe_list[newPipe].pipe[i]);
//     }
//     #endif

//     free(text);

//     return 0;
// }
