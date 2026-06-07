#include <stdio.h>
#include <stdlib.h>
#include "algorithms.h"

/*
 * Complejidad espacial del archivo: O(1)
 * No se crean estructuras adicionales.
 *
 * GREEDY (Voraz): Toma la mejor decisión local en cada paso
 * sin considerar consecuencias futuras.
 * Aquí se aplica a la asignación de memoria y planificación de procesos.
 */

// Elige el algoritmo de asignación según la política indicada
// 0 = first fit, 1 = best fit, 2 = worst fit
// Complejidad temporal: O(n) | Complejidad espacial: O(1)
int greedy_allocate(MemoryManager* mm, int pid, int size, int politica) {
    if (!mm || size <= 0) return 0;
    switch (politica) {
        case 0: return mm_allocate_first_fit(mm, pid, size);
        case 1: return mm_allocate_best_fit(mm, pid, size);
        case 2: return mm_allocate_worst_fit(mm, pid, size);
        default:
            printf("Politica desconocida, usando first fit\n");
            return mm_allocate_first_fit(mm, pid, size);
    }
}

// Greedy para scheduling: elige el proceso con menor burst_time (SJF)
// Retorna el índice del proceso más corto en el arreglo
// Complejidad temporal: O(n) | Complejidad espacial: O(1)
int greedy_shortest_job(Process* processes, int n) {
    if (!processes || n <= 0) return -1;
    int min = -1;
    for (int i = 0; i < n; i++) {
        if (processes[i].state == READY) {
            if (min == -1 || processes[i].burst_time < processes[min].burst_time)
                min = i;
        }
    }
    return min;
}

// Greedy para memoria: elige el bloque con menor desperdicio (best fit)
// Retorna el bloque más ajustado disponible
// Complejidad temporal: O(n) | Complejidad espacial: O(1)
MemoryBlock* greedy_best_block(MemoryManager* mm, int size) {
    if (!mm || size <= 0) return NULL;
    MemoryBlock* mejor = NULL;
    int menor_diff = -1;
    for (MemoryBlock* cur = mm->head; cur; cur = cur->next) {
        if (cur->free && cur->size >= size) {
            int diff = cur->size - size;
            if (menor_diff == -1 || diff < menor_diff) {
                mejor = cur;
                menor_diff = diff;
            }
        }
    }
    return mejor;
}

// Greedy para memoria: elige el bloque más grande disponible (worst fit)
// Retorna el bloque con mayor tamaño libre
// Complejidad temporal: O(n) | Complejidad espacial: O(1)
MemoryBlock* greedy_worst_block(MemoryManager* mm, int size) {
    if (!mm || size <= 0) return NULL;
    MemoryBlock* peor = NULL;
    int mayor = -1;
    for (MemoryBlock* cur = mm->head; cur; cur = cur->next) {
        if (cur->free && cur->size >= size) {
            if (mayor == -1 || cur->size > mayor) {
                peor = cur;
                mayor = cur->size;
            }
        }
    }
    return peor;
}