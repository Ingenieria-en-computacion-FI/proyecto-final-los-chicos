#include <stdio.h>
#include <stdlib.h>
#include "doubly_linked_list.h"

// Inserta new_block despues de ref. Si ref es NULL inserta al inicio
// Complejidad: O(1)
void dll_insert_after(MemoryManager* mm, MemoryBlock* ref, MemoryBlock* new_block) {
    if (!mm || !new_block) return;

    if (!ref) {
        new_block->next = mm->head;
        new_block->prev = NULL;
        if (mm->head) mm->head->prev = new_block;
        mm->head = new_block;
        return;
    }

    new_block->next = ref->next;
    new_block->prev = ref;
    if (ref->next) ref->next->prev = new_block;
    ref->next = new_block;
}

// Desconecta y libera un bloque de la lista
// Complejidad: O(1)
void dll_remove_block(MemoryManager* mm, MemoryBlock* block) {
    if (!mm || !block) return;

    if (block->prev) block->prev->next = block->next;
    else             mm->head          = block->next;

    if (block->next) block->next->prev = block->prev;

    free(block);
}

// Busca hacia atras el primer bloque libre
// Complejidad: O(n)
MemoryBlock* dll_prev_free(MemoryBlock* block) {
    MemoryBlock* cur = block ? block->prev : NULL;
    while (cur && !cur->free) cur = cur->prev;
    return cur;
}

// Busca hacia adelante el primer bloque libre
// Complejidad: O(n)
MemoryBlock* dll_next_free(MemoryBlock* block) {
    MemoryBlock* cur = block ? block->next : NULL;
    while (cur && !cur->free) cur = cur->next;
    return cur;
}

// Muestra todos los bloques con su estado
// Complejidad: O(n)
void dll_print(const MemoryManager* mm) {
    if (!mm || !mm->head) {
        printf("[Memoria vacia]\n");
        return;
    }
    MemoryBlock* cur = mm->head;
    while (cur) {
        printf("[start:%d | size:%d | %s | PID:%d]\n",
               cur->start, cur->size,
               cur->free ? "LIBRE" : "OCUP.",
               cur->pid);
        cur = cur->next;
    }
}