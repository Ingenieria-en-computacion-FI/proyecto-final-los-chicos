#include <stdio.h>
#include <stdlib.h>
#include "stack.h"
 
Stack* stack_create() {
    Stack* stack = malloc(sizeof(Stack));
    if (!stack) {
        fprintf(stderr, "Error: no se pudo crear la pila\n");
        return NULL;
    }
    stack->top  = NULL;
    stack->size = 0;
    return stack;
}
 
// Inserta un valor en el tope

void stack_push(Stack* stack, int value) {
    if (!stack) return;
 
    Node* node = malloc(sizeof(Node));
    if (!node) {
        fprintf(stderr, "Error: no se pudo hacer push\n");
        return;
    }
    node->value = value;
    node->next  = stack->top;
    stack->top  = node;
    stack->size++;
}
 
// Saca un valor dell tope y devuelve el que se posiciona o apunta "top"

int stack_pop(Stack* stack) {
    if (!stack || stack_is_empty(stack)) {
        fprintf(stderr, "Error: pila vacía (pop)\n");
        return -1;
    }
    Node* tmp  = stack->top;
    int value  = tmp->value;
    stack->top = tmp->next;
    stack->size--;
    free(tmp);
    return value;
}
 
// Devuelve el tope sin extraerlo(sin hacer pop)

int stack_peek(Stack* stack) {
    if (!stack || stack_is_empty(stack)) {
        fprintf(stderr, "Error: pila vacía (peek)\n");
        return -1;
    }
    return stack->top->value;
}
 
// Verifica si la pila esta vacia

int stack_is_empty(Stack* stack) {
    if (!stack) return 1;
    return stack->size == 0;
}
 
// Libera todos los nodos de la pila

void stack_destroy(Stack* stack) {
    if (!stack) return;
    while (!stack_is_empty(stack)) {
        stack_pop(stack);
    }
    free(stack);
}


// Todas las llamadas a las funciones conllevan una complejidad en notación big O f(n) = O(n)-> Lineal