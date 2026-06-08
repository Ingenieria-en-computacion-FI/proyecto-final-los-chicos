#ifndef TIMER_H
#define TIMER_H

#include <time.h>

typedef struct {
    struct timespec start;
    struct timespec end;
} Timer;

void   timer_start      (Timer *t);
void   timer_stop       (Timer *t);
double timer_elapsed_ms (const Timer *t);
void   timer_print      (const Timer *t, const char *label);

#endif