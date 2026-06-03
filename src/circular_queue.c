#include <stdio.h>
#include <stdlib.h>
#include "circular_queue.h"

CircularQueue* cqueue_create(void) { //Asignación de memoria para la cola circular 
    CircularQueue* cq = malloc(sizeof(CircularQueue));
    if (!cq) return NULL;
    cq->head = 0;
    cq->tail = 0;
    cq->size = 0;
    return cq;
}
// Inserta un proceso en el final de la cola
int cqueue_enqueue(CircularQueue* cq, Process p) { 
    if (!cq || cqueue_is_full(cq)) return 0;
    cq->data[cq->tail] = p;
    cq->tail = (cq->tail + 1) % CIRCULAR_QUEUE_MAX;
    cq->size++;
    return 1;
}
// Extrae un proceso al principio de la cola
int cqueue_dequeue(CircularQueue* cq, Process* out) {
    if (!cq || cqueue_is_empty(cq)) return 0;
    *out = cq->data[cq->head];
    cq->head = (cq->head + 1) % CIRCULAR_QUEUE_MAX;
    cq->size--;
    return 1;
}

int cqueue_peek(const CircularQueue* cq, Process* out) {
    if (!cq || cqueue_is_empty(cq)) return 0;
    *out = cq->data[cq->head];
    return 1;
}

int cqueue_is_empty(const CircularQueue* cq) {
    return !cq || cq->size == 0;
}

int cqueue_is_full(const CircularQueue* cq) {
    return cq && cq->size == CIRCULAR_QUEUE_MAX;
}

int cqueue_size(const CircularQueue* cq) {
    return cq ? cq->size : 0;
}

void cqueue_print(const CircularQueue* cq) {
    if (!cq || cqueue_is_empty(cq)) {
        printf("[Cola circular vacia]\n");
        return;
    }
    for (int i = 0; i < cq->size; i++) {
        int idx = (cq->head + i) % CIRCULAR_QUEUE_MAX;
        printf("[PID:%d] ", cq->data[idx].pid);
    }
    printf("\n");
}

void cqueue_destroy(CircularQueue* cq) {
    free(cq);
}