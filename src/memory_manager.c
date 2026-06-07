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

// Busca el primer hueco libre con tamaño suficiente y lo asigna al proceso
// Si el bloque es más grande lo parte en dos para no desperdiciar memoria
// Complejidad temporal: O(n) | Complejidad espacial: O(1)
int mm_allocate_first_fit(MemoryManager* mm, int pid, int size) {
    if (!mm) return 0;
    for (MemoryBlock* cur = mm->head; cur; cur = cur->next) {
        if (!cur->free || cur->size < size) continue;
        if (cur->size > size) {
            MemoryBlock* resto = malloc(sizeof(MemoryBlock));
            if (!resto) return 0;
            resto->start = cur->start + size;
            resto->size  = cur->size - size;
            resto->free  = 1;
            resto->pid   = -1;
            resto->next  = cur->next;
            resto->prev  = cur;
            if (cur->next) cur->next->prev = resto;
            cur->next = resto;
        }
        cur->size = size;
        cur->free = 0;
        cur->pid  = pid;
        return 1;
    }
    return 0;
}

// Marca como libres todos los bloques del proceso y fusiona los adyacentes
// Complejidad temporal: O(n) | Complejidad espacial: O(1)
void mm_free(MemoryManager* mm, int pid) {
    if (!mm) return;
    for (MemoryBlock* cur = mm->head; cur; cur = cur->next)
        if (cur->pid == pid && !cur->free) { cur->free = 1; cur->pid = -1; }
    mm_coalesce(mm);
}

// Une bloques libres adyacentes para reducir la fragmentación
// Complejidad temporal: O(n) | Complejidad espacial: O(1)
void mm_coalesce(MemoryManager* mm) {
    if (!mm) return;
    MemoryBlock* cur = mm->head;
    while (cur && cur->next) {
        if (cur->free && cur->next->free) {
            MemoryBlock* sig = cur->next;
            cur->size += sig->size;
            cur->next  = sig->next;
            if (sig->next) sig->next->prev = cur;
            free(sig);
        } else {
            cur = cur->next;
        }
    }
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