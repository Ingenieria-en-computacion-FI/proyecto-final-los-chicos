#include <stdio.h>
#include <stdlib.h>
#include "circular_queue.h"

/*
 * Complejidad espacial del archivo: O(1)
 * El arreglo data[CIRCULAR_QUEUE_MAX] es fijo,
 * no crece con n. El espacio es siempre constante
 * sin importar cuántos elementos haya.
 */

CircularQueue* cqueue_create(void) { //Asignación de memoria para la cola circular. Complejidad temporal: O(1)
    CircularQueue* cq = malloc(sizeof(CircularQueue));
    if (!cq) return NULL;
    cq->head = 0;
    cq->tail = 0;
    cq->size = 0;
    return cq;
}
// Inserta un proceso en el final de la cola. Complejidad temporal: O(1)
int cqueue_enqueue(CircularQueue* cq, Process p) { 
    if (!cq || cqueue_is_full(cq)) return 0;
    cq->data[cq->tail] = p;
    cq->tail = (cq->tail + 1) % CIRCULAR_QUEUE_MAX;
    cq->size++;
    return 1;
}
// Extrae un proceso al principio de la cola. Complejidad temporal: O(1)
int cqueue_dequeue(CircularQueue* cq, Process* out) {
    if (!cq || cqueue_is_empty(cq)) return 0;
    *out = cq->data[cq->head];
    cq->head = (cq->head + 1) % CIRCULAR_QUEUE_MAX;
    cq->size--;
    return 1;
}

// Copia el proceso del frente en *out sin extraerlo | O(1)
int cqueue_peek(const CircularQueue* cq, Process* out) {
    if (!cq || cqueue_is_empty(cq)) return 0;
    *out = cq->data[cq->head];
    return 1;
}

// Retorna 1 si la cola está vacía, 0 si tiene elementos | O(1)
int cqueue_is_empty(const CircularQueue* cq) {
    return !cq || cq->size == 0;
}

// Retorna el valor cuando se llena la cola | O(1)
int cqueue_is_full(const CircularQueue* cq) {
    return cq && cq->size == CIRCULAR_QUEUE_MAX;
}

// Retorna el número de procesos en la cola | O(1)
int cqueue_size(const CircularQueue* cq) {
    return cq ? cq->size : 0;
}

// Imprime todos los procesos de frente a final | O(n)
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

// Libera todos los nodos y la cola | O(n)
void cqueue_destroy(CircularQueue* cq) {
    free(cq);
}