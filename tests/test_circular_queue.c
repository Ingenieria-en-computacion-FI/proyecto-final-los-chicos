#include <assert.h>
#include <stdio.h>
#include "circular_queue.h"
#include "process.h"

void test_round_robin_behavior() {
    CircularQueue* cq = cqueue_create();

    Process p1 = process_create(1, 5, 1, 100);
    Process p2 = process_create(2, 3, 1, 100);
    Process p3 = process_create(3, 7, 1, 100);
    Process p4 = process_create(4, 2, 1, 100);

    cqueue_enqueue(cq, p1);
    cqueue_enqueue(cq, p2);
    cqueue_enqueue(cq, p3);

    Process out;
    cqueue_dequeue(cq, &out); assert(out.pid == 1);

    cqueue_enqueue(cq, p4);

    cqueue_dequeue(cq, &out); assert(out.pid == 2);
    cqueue_dequeue(cq, &out); assert(out.pid == 3);
    cqueue_dequeue(cq, &out); assert(out.pid == 4);

    cqueue_destroy(cq);
    printf("OK test_round_robin_behavior\n");
}

void test_cqueue_empty() {
    CircularQueue* cq = cqueue_create();

    assert(cqueue_is_empty(cq) == 1);
    Process p = process_create(1, 5, 1, 100);
    cqueue_enqueue(cq, p);
    assert(cqueue_is_empty(cq) == 0);

    cqueue_destroy(cq);
    printf("OK test_cqueue_empty\n");
}

int main() {
    test_round_robin_behavior();
    test_cqueue_empty();
    printf("Todos los tests de circular_queue pasaron\n");
    return 0;
}
