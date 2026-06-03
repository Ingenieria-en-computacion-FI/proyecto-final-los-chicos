#ifndef CIRCULAR_QUEUE_H
#define CIRCULAR_QUEUE_H
 
#include "process.h"
 
#define CIRCULAR_QUEUE_MAX 256
 
typedef struct {
    Process data[CIRCULAR_QUEUE_MAX];
    int head; 
    int tail;   
    int size;   
} CircularQueue;
 
CircularQueue *cqueue_create(void);
void cqueue_destroy(CircularQueue *cq);
 
int cqueue_enqueue(CircularQueue *cq, Process p);
int cqueue_dequeue(CircularQueue *cq, Process *out);
int cqueue_peek(const CircularQueue *cq, Process *out);
 
int cqueue_is_empty(const CircularQueue *cq);
int cqueue_is_full(const CircularQueue *cq);
int cqueue_size(const CircularQueue *cq);
void cqueue_print(const CircularQueue *cq);

#endif
