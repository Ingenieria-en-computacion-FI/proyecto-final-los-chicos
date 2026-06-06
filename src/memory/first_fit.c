#include <stdio.h>
#include <stdlib.h>
#include "memory_manager.h"

/* * ==============================================================================
 * AVISO PARA SCHEDULER: 
 * Por diseño de la plantilla, esta función NO recibe el PID, solo el tamaño.
 * Retorna la posición 'start' del bloque asignado (o -1 si no hay espacio).
 * * ¡OBLIGATORIO!: Después de llamar a esta función con éxito, DEBES buscar 
 * el bloque en la lista y asignarle el 'pid' manualmente (bloque->pid = tu_pid). 
 * Si no lo haces, mm_free() no sabrá qué memoria liberar cuando el proceso termine.
 * ==============================================================================
 */
int mm_allocate_first_fit(
    MemoryManager* mm,
    int size
) {
    if (mm == NULL || mm->head == NULL || size <= 0) {
        return -1; // Fallo de seguridad
    }

    MemoryBlock* bloque = mm->head;

    while (bloque != NULL) {
        // Buscamos el primer hueco que sea igual o mayor al tamaño
        if (bloque->free == 1 && bloque->size >= size) {
            
            // Si el bloque es más grande, lo partimos (split)
            if (bloque->size > size) {
                MemoryBlock* blosig = (MemoryBlock*)malloc(sizeof(MemoryBlock));
                if (blosig == NULL) return -1; // Error de memoria dinámica

                // Ajustamos atributos del bloque sobrante
                blosig->start = bloque->start + size;
                blosig->size = bloque->size - size;
                blosig->free = 1;
                blosig->pid = -1;

                // Enganchamos blosig a la lista doblemente ligada
                blosig->next = bloque->next;
                blosig->prev = bloque;

                if (bloque->next != NULL) {
                    bloque->next->prev = blosig;
                }
                bloque->next = blosig;
            }

            // Actualizamos el bloque actual para asignarlo
            bloque->size = size;
            bloque->free = 0;
            
            // Retornamos la posición (start) del bloque asignado como indicador de éxito
            return bloque->start; 
        }
        bloque = bloque->next;
    }

    return -1; // Retorna -1 si no hubo espacio suficiente (Fallo)
}