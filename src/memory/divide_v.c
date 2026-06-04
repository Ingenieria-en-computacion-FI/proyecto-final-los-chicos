#include <stdio.h>
#include <stdlib.h>
#include "memory_manager.h"

/*
 * Estrategia Divide y Vencerás: Mueve todos los procesos al inicio de la memoria
 * y fusiona todos los huecos libres en un solo gran bloque al final.
 */
void mm_divide_v(MemoryManager* mm) {
    if (mm == NULL || mm->head == NULL) {
        return;
    }

    MemoryBlock* bloque = mm->head;
    MemoryBlock* ocu = NULL;
    
    int totalL = 0;
    int ubiactual = 0; // Nos dirá en qué byte (start) debe empezar el siguiente bloque

    // PASO 1: Desplazar todos los bloques OCUPADOS hacia el inicio
    while (bloque != NULL) {
        MemoryBlock* sigtemp = bloque->next;

        if (bloque->free == 0) { // Si el bloque tiene un proceso
            // Recalculamos su inicio como si lo empujáramos hacia la izquierda
            bloque->start = ubiactual;
            ubiactual += bloque->size; 

            // Lo desenlazamos temporalmente
            bloque->next = NULL;
            bloque->prev = NULL;

            // Lo volvemos a enlazar al principio
            if (ocu == NULL) {
                mm->head = bloque; 
            } else {
                ocu->next = bloque;
                bloque->prev = ocu;
            }
            ocu = bloque;

        } else {
            // Si el bloque está libre, sumamos su tamaño a nuestro "costal" y lo destruimos
            totalL += bloque->size;
            free(bloque);
        }

        bloque = sigtemp; 
    }

    // PASO 2: Crear el bloque gigante con todo el espacio libre recolectado al final
    if (totalL > 0) {
        MemoryBlock* ultilibre = (MemoryBlock*)malloc(sizeof(MemoryBlock));
        if (ultilibre != NULL) {
            ultilibre->start = ubiactual;
            ultilibre->size = totalL;
            ultilibre->free = 1;
            ultilibre->pid = -1;
            ultilibre->next = NULL;
            ultilibre->prev = ocu;

            // Enganchamos el bloque libre final a los ocupados
            if (ocu != NULL) {
                ocu->next = ultilibre;
            } else {
                mm->head = ultilibre; // Por si la memoria estaba 100% vacía
            }
        }
    }
}