#include <stdio.h>
#include <time.h>
#include "timer.h"
 
void timer_start(Timer *t) {
    if (!t) return;
    clock_gettime(CLOCK_MONOTONIC, &t->start);
}
 
void timer_stop(Timer *t) {
    if (!t) return;
    clock_gettime(CLOCK_MONOTONIC, &t->end);
}
 
double timer_elapsed_ms(const Timer *t) {
    if (!t) return 0.0;
    double sec  = (double)(t->end.tv_sec  - t->start.tv_sec);
    double nsec = (double)(t->end.tv_nsec - t->start.tv_nsec);
    return sec * 1000.0 + nsec / 1e6;
}
 
void timer_print(const Timer *t, const char *label) {
    if (!t || !label) return;
    printf("[TIMER] %s: %.4f ms\n", label, timer_elapsed_ms(t));
}
 