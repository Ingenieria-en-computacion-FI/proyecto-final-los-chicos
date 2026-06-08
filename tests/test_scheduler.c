#include <assert.h>
#include <stdio.h>
#include "scheduler.h"

void test_fifo_scheduler() {
    Process processes[3];
    processes[0] = process_create(1, 5, 1, 100);
    processes[1] = process_create(2, 3, 1, 100);
    processes[2] = process_create(3, 7, 1, 100);

    LinkedList* finished = list_create();
    SchedulerResult r = scheduler_fifo(processes, 3, finished);

    assert(r.total_processes == 3);
    assert(r.total_time == 15); // 5+3+7
    assert(list_size(finished) == 3);

    list_destroy(finished);
    printf("OK test_fifo_scheduler\n");
}

void test_round_robin_scheduler() {
    Process processes[2];
    processes[0] = process_create(1, 5, 1, 100);
    processes[1] = process_create(2, 3, 1, 100);

    LinkedList* finished = list_create();
    SchedulerResult r = scheduler_round_robin(processes, 2, 2, finished);

    assert(r.total_processes == 2);
    assert(r.total_time == 8); // 5+3
    assert(list_size(finished) == 2);

    list_destroy(finished);
    printf("OK test_round_robin_scheduler\n");
}

void test_sjf_scheduler() {
    Process processes[3];
    processes[0] = process_create(1, 10, 1, 100);
    processes[1] = process_create(2, 2,  1, 100);
    processes[2] = process_create(3, 5,  1, 100);

    LinkedList* finished = list_create();
    SchedulerResult r = scheduler_sjf(processes, 3, finished);

    // SJF ejecuta en orden: P2(2), P3(5), P1(10)
    assert(r.total_processes == 3);
    assert(r.total_time == 17); // 2+5+10

    list_destroy(finished);
    printf("OK test_sjf_scheduler\n");
}

int main() {
    test_fifo_scheduler();
    test_round_robin_scheduler();
    test_sjf_scheduler();
    printf("Todos los tests de scheduler pasaron\n");
    return 0;
}