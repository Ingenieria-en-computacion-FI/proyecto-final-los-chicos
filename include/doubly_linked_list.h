#ifndef DOUBLY_LINKED_LIST_H
#define DOUBLY_LINKED_LIST_H
 
// Expone operaciones fundamentales en bidireccionalidad
 
#include "memory_manager.h"
 

void dll_insert_after(MemoryManager *mm, MemoryBlock *ref, MemoryBlock *new_block); // Inserta un nuevo bloque
 
void dll_remove_block(MemoryManager *mm, MemoryBlock *block); // Elimina y libera un bloque
 
MemoryBlock *dll_prev_free(MemoryBlock *block); //Recorre hacia atras
 
MemoryBlock *dll_next_free(MemoryBlock *block); // Recorre hacía adelante
 
void dll_print(const MemoryManager *mm); // Imprime la lista completa
 
#endif