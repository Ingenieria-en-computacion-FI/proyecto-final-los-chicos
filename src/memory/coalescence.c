#include <stdio.h>
#include <stdlib.h>
#include "memory_manager.h"

/*
 * Recorre toda la lista de memoria del MemoryManager
 * y fusiona cualquier grupo de bloques libres contiguos.
 */
void mm_coalesce(MemoryManager* mm) {
    // Validación de seguridad por si el administrador no está inicializado
    if (mm == NULL || mm->head == NULL) {
        return; 
    }

    MemoryBlock* bloque = mm->head;

    // Recorremos la lista de principio a fin
    while (bloque != NULL) {
        
        // Si estamos parados en un bloque libre y el de la derecha también está libre...
        if (bloque->free == 1 && bloque->next != NULL && bloque->next->free == 1) {
            MemoryBlock* blosig = bloque->next;

            // 1. Absorbemos el tamaño del bloque siguiente
            bloque->size += blosig->size;

            // 2. Reconectamos los punteros para "brincarnos" el nodo absorbido
            bloque->next = blosig->next;

            if (blosig->next != NULL) {
                blosig->next->prev = bloque;
            }

            // 3. Destruimos el nodo que ya no necesitamos
            free(blosig);

            /* * IMPORTANTE: No avanzamos el puntero 'bloque' en este paso.
             * Al no avanzar, el ciclo vuelve a evaluar el mismo bloque actual
             * contra su NUEVO vecino de la derecha. Así, si hay 3 o 4 bloques libres
             * seguidos, los absorberá todos en una sola pasada.
             */
        } else {
            // Solo si no hubo fusión, avanzamos al siguiente bloque
            bloque = bloque->next;
        }
    }
}