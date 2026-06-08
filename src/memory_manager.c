#include <stdio.h>
#include <stdlib.h>
#include "memory_manager.h"

/*
 * Complejidad espacial del archivo: O(n)
 * donde n es el número de bloques de memoria en la lista.
 */

// Crea el gestor con un bloque inicial libre del tamaño total | O(1)
MemoryManager* mm_create(int total_memory) {
    MemoryManager* mm = malloc(sizeof(MemoryManager));
    if (!mm) return NULL;

    MemoryBlock* bloque = malloc(sizeof(MemoryBlock));
    if (!bloque) { free(mm); return NULL; }

    bloque->start = 0;
    bloque->size  = total_memory;
    bloque->free  = 1;
    bloque->pid   = -1;
    bloque->next  = NULL;
    bloque->prev  = NULL;

    mm->head         = bloque;
    mm->total_memory = total_memory;
    return mm;
}

// Marca como libres todos los bloques del proceso y fusiona los adyacentes
// Complejidad temporal: O(n) | Complejidad espacial: O(1)
void mm_free(MemoryManager* mm, int pid) {
    if (!mm) return;
    for (MemoryBlock* cur = mm->head; cur; cur = cur->next)
        if (cur->pid == pid && !cur->free) { cur->free = 1; cur->pid = -1; }
    mm_coalesce(mm);
}

// Libera todos los bloques y el gestor de memoria
// Complejidad temporal: O(n) | Complejidad espacial: O(1)
void mm_destroy(MemoryManager* mm) {
    if (!mm) return;
    MemoryBlock* cur = mm->head;
    while (cur) {
        MemoryBlock* tmp = cur;
        cur = cur->next;
        free(tmp);
    }
    free(mm);
}

// Muestra el estado actual de cada bloque de memoria
// Complejidad temporal: O(n) | Complejidad espacial: O(1)
void mm_print(MemoryManager* mm) {
    if (!mm) return;
    printf("Memoria total: %d\n", mm->total_memory);
    for (MemoryBlock* cur = mm->head; cur; cur = cur->next)
        printf("[start:%d | size:%d | %s | PID:%d]\n",
               cur->start, cur->size,
               cur->free ? "LIBRE" : "OCUP.", cur->pid);
}