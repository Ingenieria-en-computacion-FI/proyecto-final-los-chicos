#ifndef STACK_H
#define STACK_H

typedef struct Node {
    int value;
    struct Node *next;
} Node;

typedef struct Stack {
    struct Node *top;
    int size;
} Stack;

Stack* stack_create();
void stack_push(Stack* stack, int value);
int stack_pop(Stack* stack);
int stack_peek(Stack* stack);
int stack_is_empty(Stack* stack);
void stack_destroy(Stack* stack);

#endif 