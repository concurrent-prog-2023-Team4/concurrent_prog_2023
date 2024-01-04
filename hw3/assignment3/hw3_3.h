#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include "../monitor_lib/monitors.h"


#define ANSI_COLOR_RED     "\x1b[31m"   // define color codes to print TCL messages //
#define ANSI_COLOR_RESET   "\x1b[0m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_GREEN "\x1b[32m"
// #define DEBUG

Monitor *bridge_monitor;

int blues_num, reds_num;
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