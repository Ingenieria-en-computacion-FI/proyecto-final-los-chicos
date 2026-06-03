#ifndef QUEUE_H
#define QUEUE_H
 
#include "process.h"
 
// Nodo interno
typedef struct QueueNode {
    Process process;
    struct QueueNode *next;
} QueueNode;
 
// Cola simple (FIFO)
typedef struct {
    QueueNode *front;   
    QueueNode *rear;    
    int size;
} Queue;
 
Queue *queue_create(void);
void   queue_destroy(Queue *q);
 
int  queue_enqueue(Queue *q, Process p);       
int  queue_dequeue(Queue *q, Process *out);     
int  queue_peek(const Queue *q, Process *out); 
 
int  queue_is_empty(const Queue *q);
int  queue_size(const Queue *q);
void queue_print(const Queue *q);
 
#endif 