#include <stdio.h>
#include <stdlib.h>
#include "memory_manager.h"

// 1. NACIMIENTO: Construye el cuarto vacío
MemoryManager* mm_create(int total_memory) {
    // Reservamos la memoria principal para el Administrador
    MemoryManager* mm = (MemoryManager*)malloc(sizeof(MemoryManager));
    if (mm == NULL) {
        return NULL; 
    }
    mm->total_memory = total_memory; // <--- Mantenemos el nombre original

    // Creamos el primer y único bloque de memoria (bloini)
    MemoryBlock* bloini = (MemoryBlock*)malloc(sizeof(MemoryBlock));
    if (bloini == NULL) {
        free(mm);
        return NULL;
    }

    // Configuramos el bloque gigante
    bloini->start = 0;
    bloini->size = total_memory;
    bloini->free = 1;      // 1 significa que está libre
    bloini->pid = -1;      // -1 significa que no tiene dueño
    bloini->next = NULL;   
    bloini->prev = NULL;   

    // Enganchamos este bloque como la cabeza de nuestra lista
    mm->head = bloini;

    return mm;
}

// 2. LIMPIEZA: Desocupa y une los pedazos
void mm_free(MemoryManager* mm, int pid) {
    if (mm == NULL || mm->head == NULL) {
        return;
    }

    MemoryBlock* bloque = mm->head;

    // Recorremos toda la RAM buscando al proceso
    while (bloque != NULL) {
        // Si el bloque le pertenece al proceso que está terminando...
        if (bloque->pid == pid && bloque->free == 0) {
            bloque->free = 1;  // Lo desocupamos
            bloque->pid = -1;  // Le quitamos la etiqueta del dueño
        }
        bloque = bloque->next;
    }

    // Llamamos a tu recolector de basura para que fusione los huecos libres contiguos.
    mm_coalesce(mm);
}

// 3. DEMOLICIÓN: Evita fugas de memoria al apagar el simulador
void mm_destroy(MemoryManager* mm) {
    if (mm == NULL) {
        return;
    }

    MemoryBlock* bloque = mm->head;
    MemoryBlock* sigtemp;

    // Recorremos la lista destruyendo nodo por nodo
    while (bloque != NULL) {
        sigtemp = bloque->next;  // Guardamos la referencia para no perder la cadena
        free(bloque);            // Destruimos el nodo actual
        bloque = sigtemp;        // Avanzamos
    }

    // Finalmente, destruimos la estructura del administrador
    free(mm);
}