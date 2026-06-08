#include <assert.h>
#include <stdio.h>
#include "stack.h"

void test_stack_push_pop() {
    Stack* stack = stack_create();

    stack_push(stack, 10);
    stack_push(stack, 20);

    assert(stack_pop(stack) == 20);
    assert(stack_pop(stack) == 10);

    stack_destroy(stack);
    printf("OK test_stack_push_pop\n");
}

void test_stack_empty() {
    Stack* stack = stack_create();

    assert(stack_is_empty(stack) == 1);
    stack_push(stack, 5);
    assert(stack_is_empty(stack) == 0);

    stack_destroy(stack);
    printf("OK test_stack_empty\n");
}

void test_stack_peek() {
    Stack* stack = stack_create();

    stack_push(stack, 42);
    assert(stack_peek(stack) == 42);
    assert(stack_pop(stack) == 42);

    stack_destroy(stack);
    printf("OK test_stack_peek\n");
}

int main() {
    test_stack_push_pop();
    test_stack_empty();
    test_stack_peek();
    printf("Todos los tests de stack pasaron\n");
    return 0;
}
