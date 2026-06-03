#include <stdio.h>
#include <stdlib.h>
#include "memory_manager.h"

MemoryManager* mm_create(
    int total_memory
) {
    // 1. Reservamos la estructura principal
    MemoryManager* mm = (MemoryManager*)malloc(sizeof(MemoryManager));
    if (mm == NULL) return NULL; 
    
    mm->total_memory = total_memory;

    // 2. Creamos el megabloque inicial (bloini)
    MemoryBlock* bloini = (MemoryBlock*)malloc(sizeof(MemoryBlock));
    if (bloini == NULL) {
        free(mm);
        return NULL;
    }

    // 3. Configuramos los atributos obligatorios
    bloini->start = 0;
    bloini->size = total_memory;
    bloini->free = 1;
    bloini->pid = -1;
    bloini->next = NULL;
    bloini->prev = NULL;

    mm->head = bloini;

    return mm; // Retornamos el administrador creado
}

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

void mm_free(
    MemoryManager* mm,
    int pid
) {
    if (mm == NULL || mm->head == NULL) return;

    MemoryBlock* bloque = mm->head;

    // Buscamos y liberamos la memoria del proceso
    while (bloque != NULL) {
        if (bloque->pid == pid && bloque->free == 0) {
            bloque->free = 1;
            bloque->pid = -1;
        }
        bloque = bloque->next;
    }

    // Desfragmentación automática
    mm_coalesce(mm);
}

void mm_coalesce(
    MemoryManager* mm
) {
    if (mm == NULL || mm->head == NULL) return;

    MemoryBlock* bloque = mm->head;

    while (bloque != NULL) {
        // Si el bloque actual y el siguiente están libres, los unimos
        if (bloque->free == 1 && bloque->next != NULL && bloque->next->free == 1) {
            MemoryBlock* blosig = bloque->next;

            bloque->size += blosig->size;
            bloque->next = blosig->next;

            if (blosig->next != NULL) {
                blosig->next->prev = bloque;
            }

            free(blosig); // Destruimos el sobrante
            // No avanzamos el puntero para que siga verificando si hay más huecos adelante
        } else {
            bloque = bloque->next;
        }
    }
}

void mm_destroy(
    MemoryManager* mm
) {
    if (mm == NULL) return;

    MemoryBlock* bloque = mm->head;
    MemoryBlock* sigtemp;

    // Liberamos nodo por nodo
    while (bloque != NULL) {
        sigtemp = bloque->next;
        free(bloque);
        bloque = sigtemp;
    }

    // Finalmente destruimos el administrador
    free(mm);
}

/* * ==============================================================================
 * AVISO PARA SCHEDULER: 
 * Por diseño de la plantilla, esta función NO recibe el PID, solo el tamaño.
 * Retorna la posición 'start' del bloque asignado (o -1 si no hay espacio).
 * * ¡OBLIGATORIO!: Después de llamar a esta función con éxito, DEBES buscar 
 * el bloque en la lista y asignarle el 'pid' manualmente (bloque->pid = tu_pid). 
 * Si no lo haces, mm_free() no sabrá qué memoria liberar cuando el proceso termine.
es como lo explica la ia, mejor de lo que yo lo haria
 * ==============================================================================
 */