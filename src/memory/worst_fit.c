#include <stdio.h>
#include <stdlib.h>
#include "memory_manager.h" 

// Asigna el hueco libre más grande disponible al proceso
// Complejidad temporal: O(n) | Complejidad espacial: O(1)

int mm_allocate_worst_fit(MemoryManager* mm, int pid, int size) {
    if (mm == NULL || mm->head == NULL || size <= 0) return -1;

    MemoryBlock* bloque = mm->head;
    MemoryBlock* peor_bloque = NULL;
    int mayor_tamano = -1;

    // 1. Fase de búsqueda: Buscamos el bloque más masivo de toda la RAM
    while (bloque != NULL) {
        if (bloque->free == 1 && bloque->size >= size) {
            
            // Si es el primero que cabe o si es más grande que el campeón actual
            if (peor_bloque == NULL || bloque->size > mayor_tamano) {
                peor_bloque = bloque;
                mayor_tamano = bloque->size;
            }
        }
        bloque = bloque->next;
    }

    // 2. Fase de asignación: Partimos el bloque gigante y lo asignamos
    if (peor_bloque != NULL) {
        if (peor_bloque->size > size) {
            MemoryBlock* blosig = (MemoryBlock*)malloc(sizeof(MemoryBlock));
            if (blosig == NULL) return -1; 

            blosig->start = peor_bloque->start + size;
            blosig->size = peor_bloque->size - size;
            blosig->free = 1;
            blosig->pid = -1;

            blosig->next = peor_bloque->next;
            blosig->prev = peor_bloque;

            if (peor_bloque->next != NULL) {
                peor_bloque->next->prev = blosig;
            }
            peor_bloque->next = blosig;
        }

        peor_bloque->size = size;
        peor_bloque->free = 0;
        peor_bloque->pid = pid;
        
        return peor_bloque->start;
    }

    return -1;
}