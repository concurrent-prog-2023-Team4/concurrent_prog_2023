// Header file for hw1.1

typedef struct pipe_info {
    int size;
    int write;
    int read;
    char *pipe;
} pipe_info;

void pipe_info_init(pipe_info pipe_struct);

int pipe_open(int size);

int pipe_write(int p, char c);

int pipe_writeDone(int p);

int pipe_read(int p, char *c);
