#include <assert.h>
#include <stdio.h>
#include "memory_manager.h"

void test_first_fit() {
    MemoryManager* mm = mm_create(1024);

    int r1 = mm_allocate_first_fit(mm, 1, 100);
    int r2 = mm_allocate_first_fit(mm, 2, 200);

    assert(r1 >= 0);
    assert(r2 >= 0);

    mm_free(mm, 1);

    int r3 = mm_allocate_first_fit(mm, 3, 50);
    assert(r3 == r1); // reutiliza el espacio liberado

    mm_destroy(mm);
    printf("OK test_first_fit\n");
}

void test_coalesce() {
    MemoryManager* mm = mm_create(1024);

    mm_allocate_first_fit(mm, 1, 100);
    mm_allocate_first_fit(mm, 2, 100);
    mm_allocate_first_fit(mm, 3, 100);

    mm_free(mm, 1);
    mm_free(mm, 2);
    mm_coalesce(mm);

    // Después de coalescer debe haber un bloque libre de 200
    assert(mm->head->free == 1);
    assert(mm->head->size == 200);

    mm_destroy(mm);
    printf("OK test_coalesce\n");
}

void test_no_space() {
    MemoryManager* mm = mm_create(100);

    int r = mm_allocate_first_fit(mm, 1, 200);
    assert(r == -1); // no hay espacio

    mm_destroy(mm);
    printf("OK test_no_space\n");
}

int main() {
    test_first_fit();
    test_coalesce();
    test_no_space();
    printf("Todos los tests de memory_manager pasaron\n");
    return 0;
}