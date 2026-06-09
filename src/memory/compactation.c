#include <stdio.h>
#include <stdlib.h>
#include "memory_manager.h"

// Mueve todos los bloques ocupados al inicio y fusiona los libres al final
// Complejidad temporal: O(n) | Complejidad espacial: O(1)
void mm_compact(MemoryManager* mm) {
    if (mm == NULL || mm->head == NULL) return;

    MemoryBlock* bloque = mm->head;
    MemoryBlock* ocu = NULL;
    
    int totalL = 0;
    int ubiactual = 0;

    // PASO 1: Desplazar todos los bloques OCUPADOS hacia el inicio
    while (bloque != NULL) {
        MemoryBlock* sigtemp = bloque->next;

        if (bloque->free == 0) {
            bloque->start = ubiactual;
            ubiactual += bloque->size;

            bloque->next = NULL;
            bloque->prev = NULL;

            if (ocu == NULL) {
                mm->head = bloque;
            } else {
                ocu->next = bloque;
                bloque->prev = ocu;
            }
            ocu = bloque;

        } else {
            totalL += bloque->size;
            free(bloque);
        }

        bloque = sigtemp;
    }

    // PASO 2: Crear el bloque gigante con todo el espacio libre al final
    if (totalL > 0) {
        MemoryBlock* ultilibre = malloc(sizeof(MemoryBlock));
        if (ultilibre != NULL) {
            ultilibre->start = ubiactual;
            ultilibre->size  = totalL;
            ultilibre->free  = 1;
            ultilibre->pid   = -1;
            ultilibre->next  = NULL;
            ultilibre->prev  = ocu;

            if (ocu != NULL) ocu->next = ultilibre;
            else             mm->head  = ultilibre;
        }
    }
}