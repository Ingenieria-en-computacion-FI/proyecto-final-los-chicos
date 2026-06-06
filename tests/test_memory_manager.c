#include <assert.h>
#include "memory_manager.h"
#include <stdio.h> 

// ... resto de tu código

void test_first_fit() {
    MemoryManager* mm = mm_create(1024);

    int block1_start = mm_allocate_first_fit(mm, 100);
    // Asignamos PID manualmente para que mm_free funcione
    mm->head->pid = 1; 
    mm->head->free = 0;

    int block2_start = mm_allocate_first_fit(mm, 200);
    mm->head->next->pid = 2;
    mm->head->next->free = 0;

    assert(block1_start >= 0);
    assert(block2_start >= 0);

    mm_free(mm, 1); // Ahora sí, liberamos el PID 1

    int block3_start = mm_allocate_first_fit(mm, 50);

    // Verificamos que se reutilizó el espacio
    assert(block3_start == block1_start);

    mm_destroy(mm);
}

int main() {
    test_first_fit();
    printf("¡Test First Fit pasado con exito!\n");
    return 0;
}