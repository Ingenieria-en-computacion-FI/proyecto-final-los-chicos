#include <stdio.h>
#include <stdlib.h>
#include "algorithms.h"

/*
 * Complejidad espacial del archivo: O(1)
 * No se crean estructuras adicionales, solo se recorre la lista existente.
 *
 * FUERZA BRUTA: Explora TODOS los bloques de memoria sin excepción
 * hasta encontrar el primero que cumpla la condición.
 * No optimiza ni descarta opciones — revisa todo.
 */

// Recorre exhaustivamente todos los bloques y retorna el primero libre con tamaño suficiente
// Complejidad temporal: O(n) | Complejidad espacial: O(1)
MemoryBlock* brute_force_find_block(MemoryManager* mm, int required) {
    if (!mm || required <= 0) return NULL;
    for (MemoryBlock* cur = mm->head; cur; cur = cur->next) {
        if (cur->free && cur->size >= required)
            return cur;
    }
    return NULL;
}

// Recorre exhaustivamente todos los bloques buscando fragmentación
// Retorna 1 si hay fragmentación (memoria libre dividida), 0 si no
// Complejidad temporal: O(n) | Complejidad espacial: O(1)
int brute_force_check_fragmentation(MemoryManager* mm, int required) {
    if (!mm) return 0;
    int total_libre = 0;
    int bloque_continuo = 0;
    for (MemoryBlock* cur = mm->head; cur; cur = cur->next) {
        if (cur->free) {
            total_libre += cur->size;
            if (cur->size > bloque_continuo)
                bloque_continuo = cur->size;
        }
    }
    // Hay fragmentación si hay memoria libre total suficiente
    // pero ningún bloque continuo es suficientemente grande
    return (total_libre >= required && bloque_continuo < required);
}

// Cuenta cuántos bloques libres hay en total recorriendo toda la lista
// Complejidad temporal: O(n) | Complejidad espacial: O(1)
int brute_force_count_free(MemoryManager* mm) {
    if (!mm) return 0;
    int count = 0;
    for (MemoryBlock* cur = mm->head; cur; cur = cur->next)
        if (cur->free) count++;
    return count;
}

// Imprime todos los bloques de memoria con su estado
// Complejidad temporal: O(n) | Complejidad espacial: O(1)
void brute_force_print_all(MemoryManager* mm) {
    if (!mm) return;
    printf("=== Estado de memoria (fuerza bruta) ===\n");
    int i = 0;
    for (MemoryBlock* cur = mm->head; cur; cur = cur->next) {
        printf("Bloque %d: start=%d size=%d %s PID=%d\n",
               i++, cur->start, cur->size,
               cur->free ? "LIBRE" : "OCUP.", cur->pid);
    }
}