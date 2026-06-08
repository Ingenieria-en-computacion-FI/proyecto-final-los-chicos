#include <assert.h>
#include <stdio.h>
#include "queue.h"
#include "process.h"

void test_queue_fifo() {
    Queue* q = queue_create();

    Process p1 = process_create(1, 5, 1, 100);
    Process p2 = process_create(2, 3, 1, 100);
    Process p3 = process_create(3, 7, 1, 100);

    queue_enqueue(q, p1);
    queue_enqueue(q, p2);
    queue_enqueue(q, p3);

    Process out;
    queue_dequeue(q, &out); assert(out.pid == 1);
    queue_dequeue(q, &out); assert(out.pid == 2);
    queue_dequeue(q, &out); assert(out.pid == 3);

    queue_destroy(q);
    printf("OK test_queue_fifo\n");
}

void test_queue_empty() {
    Queue* q = queue_create();

    assert(queue_is_empty(q) == 1);
    Process p = process_create(1, 5, 1, 100);
    queue_enqueue(q, p);
    assert(queue_is_empty(q) == 0);

    queue_destroy(q);
    printf("OK test_queue_empty\n");
}

int main() {
    test_queue_fifo();
    test_queue_empty();
    printf("Todos los tests de queue pasaron\n");
    return 0;
}