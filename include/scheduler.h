#ifndef SCHEDULER_H
#define SCHEDULER_H

#include "process.h"
#include "queue.h"
#include "circular_queue.h"
#include "linked_list.h"
#include "stack.h"

typedef struct {
    int   total_processes;
    float avg_waiting_time;
    float avg_turnaround_time;
    int   total_time;
} SchedulerResult;

SchedulerResult scheduler_fifo(Process *processes, int n, LinkedList *finished);

SchedulerResult scheduler_round_robin(Process *processes, int n, int quantum,LinkedList *finished);

SchedulerResult scheduler_sjf(Process *processes, int n, LinkedList *finished);

void scheduler_print_result(const SchedulerResult *r, const char *name);
void scheduler_reset_processes(Process *processes, int n);

#endif