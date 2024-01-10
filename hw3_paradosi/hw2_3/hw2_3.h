#include <stdio.h>
#include <pthread.h>
#include "../assignment1/mysem.h"
#include <stdlib.h>
#include <unistd.h>


#define ANSI_COLOR_RED     "\x1b[31m"   // define color codes to print TCL messages //
#define ANSI_COLOR_RESET   "\x1b[0m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
// #define DEBUG

mysem_t mtx;    // to read struct //
mysem_t r_sem;
mysem_t b_sem;
mysem_t print_sem;

struct bridge
{
    int b_waiting;
    int r_waiting;
    int max_cars;
    int on_bridge;
    int red_passed;
    int blue_passed;
    int b_down;
    int r_down;
    char color;

};
typedef struct bridge bridge_t;

bridge_t *bridge;

void *red_car(void *varg);

void *blue_car(void *varg);