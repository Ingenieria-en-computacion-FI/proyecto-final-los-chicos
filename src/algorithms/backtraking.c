#include <stdio.h>
#include <stdlib.h>
#include "algorithms.h"

/*
 * Complejidad espacial del archivo: O(n)
 * donde n es el número de bloques en la lista.
 * El snapshot guarda una copia de cada bloque.
 *
 * BACKTRACKING: Guarda el estado actual del sistema y permite
 * restaurarlo si una operación falla o produce un resultado inválido.
 * Útil para deshacer asignaciones de memoria fallidas.
 */

// Guarda una copia completa del estado actual de la memoria
// Complejidad temporal: O(n) | Complejidad espacial: O(n)
MemorySnapshot* backtrack_save(const MemoryManager* mm) {
    if (!mm) return NULL;

    // Contar bloques
    int count = 0;
    for (MemoryBlock* cur = mm->head; cur; cur = cur->next) count++;

    MemorySnapshot* snap = malloc(sizeof(MemorySnapshot));
    if (!snap) return NULL;

    snap->starts = malloc(count * sizeof(int));
    snap->sizes  = malloc(count * sizeof(int));
    snap->frees  = malloc(count * sizeof(int));
    snap->pids   = malloc(count * sizeof(int));
    snap->count  = count;

    if (!snap->starts || !snap->sizes || !snap->frees || !snap->pids) {
        backtrack_free_snapshot(snap);
        return NULL;
    }

    // Copiar estado de cada bloque
    int i = 0;
    for (MemoryBlock* cur = mm->head; cur; cur = cur->next) {
        snap->starts[i] = cur->start;
        snap->sizes[i]  = cur->size;
        snap->frees[i]  = cur->free;
        snap->pids[i]   = cur->pid;
        i++;
    }
    return snap;
}

// Restaura la memoria al estado guardado en el snapshot
// Destruye los bloques actuales y reconstruye desde el snapshot
// Complejidad temporal: O(n) | Complejidad espacial: O(n)
void backtrack_restore(MemoryManager* mm, const MemorySnapshot* snap) {
    if (!mm || !snap) return;

    // Liberar bloques actuales
    MemoryBlock* cur = mm->head;
    while (cur) {
        MemoryBlock* tmp = cur;
        cur = cur->next;
        free(tmp);
    }
    mm->head = NULL;

    // Reconstruir desde el snapshot
    MemoryBlock* prev = NULL;
    for (int i = 0; i < snap->count; i++) {
        MemoryBlock* bloque = malloc(sizeof(MemoryBlock));
        if (!bloque) return;
        bloque->start = snap->starts[i];
        bloque->size  = snap->sizes[i];
        bloque->free  = snap->frees[i];
        bloque->pid   = snap->pids[i];
        bloque->next  = NULL;
        bloque->prev  = prev;
        if (prev) prev->next = bloque;
        else      mm->head   = bloque;
        prev = bloque;
    }
}

// Libera la memoria usada por el snapshot
// Complejidad temporal: O(1) | Complejidad espacial: O(1)
void backtrack_free_snapshot(MemorySnapshot* snap) {
    if (!snap) return;
    free(snap->starts);
    free(snap->sizes);
    free(snap->frees);
    free(snap->pids);
    free(snap);
}

// Imprime el contenido del snapshot guardado
// Complejidad temporal: O(n) | Complejidad espacial: O(1)
void backtrack_print_snapshot(const MemorySnapshot* snap) {
    if (!snap) return;
    printf("=== Snapshot guardado (%d bloques) ===\n", snap->count);
    for (int i = 0; i < snap->count; i++) {
        printf("Bloque %d: start=%d size=%d %s PID=%d\n",
               i, snap->starts[i], snap->sizes[i],
               snap->frees[i] ? "LIBRE" : "OCUP.", snap->pids[i]);
    }
}