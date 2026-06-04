#include <stdio.h>
#include <stdlib.h>
#include "queue.h"

/*
 * Complejidad espacial del archivo: O(n)
 * donde n es el número de procesos en la cola.
 * Cada nodo ocupa espacio constante O(1),
 * y hay n nodos en total.
 */


// Crea una cola vacía en el heap. Complejidad temporal: O(1)

Queue *queue_create(void) {
    Queue *q = (Queue *)malloc(sizeof(Queue));
    if (!q) {
        fprintf(stderr, "Error: no se pudo crear la cola\n");
        return NULL;
    }
    q->front = NULL;
    q->rear = NULL;
    q->size = 0;
    return q;
}

// Inserta un proceso al final de la cola siguiendo FIFO. Complejidad temporal: O(1)

int queue_enqueue(Queue *q, Process p) {
    if (!q) return 0;

    QueueNode *node = (QueueNode *)malloc(sizeof(QueueNode));
    if (!node) {
        fprintf(stderr, "Error: no se pudo insertar en la cola\n");
        return 0;
    }
    node->process = p;
    node->next = NULL;

    if (q->rear == NULL) { // Verificación si la cola se encuentra vacía; es decir rear y front apuntan al mismo nodo. Complejidad temporal: O(1)
        q->front = node;
        q->rear = node;
    } else {
        q->rear->next = node;
        q->rear = node;
    }
    q->size++;
    return 1;
}

// Extrae el proceso de enfrente. Complejidad temporal: O(1)

int queue_dequeue(Queue *q, Process *out) {
    if (!q || !q->front) {
        fprintf(stderr, "Error: cola vacía (dequeue)\n");
        return 0;
    }
    QueueNode *tmp = q->front;
    *out = tmp->process;
    q->front = tmp->next;

    if (q->front == NULL) {
        q->rear = NULL;   
    }
    q->size--;
    free(tmp);
    return 1;
}

// Copia el frente. Complejidad temporal: O(1)

int queue_peek(const Queue *q, Process *out) {
    if (!q || !q->front) return 0;
    *out = q->front->process;
    return 1;
}

// Verificación de procesos. Complejidad temporal: O(1)

int queue_is_empty(const Queue *q) {
    if (!q) return 1;
    return q->size == 0;
}

int queue_size(const Queue *q) {
    if (!q) return 0;
    return q->size;
}

// Muestra todos los procesos existentes en la cola. Complejidad temporal: O(1)

void queue_print(const Queue *q) {
    if (!q || !q->front) {
        printf("[Valiste mijo. Cola vacía]\n");
        return;
    }
    QueueNode *cur = q->front;
    printf("FRONT -> ");
    while (cur) {
        printf("[PID:%d bt:%d] -> ", cur->process.pid, cur->process.burst_time);
        cur = cur->next;
    }
    printf("NULL\n");
}

// Libera todos los nodos o procesos de la cola. Complejidad temporal: O(1)

void queue_destroy(Queue *q) {
    if (!q) return;
    Process tmp;
    while (!queue_is_empty(q)) {
        queue_dequeue(q, &tmp);
    }
    free(q);
}