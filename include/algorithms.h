#ifndef ALGORITHMS_H
#define ALGORITHMS_H

#include "memory_manager.h"
#include "process.h"

// ============================================================================
// 1. FUERZA BRUTA (brute_force.c)
// ============================================================================

// Busca exhaustivamente el primer bloque libre con tamaño suficiente
MemoryBlock* brute_force_find_block(MemoryManager* mm, int required);

// Verifica si la memoria libre total es suficiente pero está fragmentada
int brute_force_check_fragmentation(MemoryManager* mm, int required);

// Cuenta la cantidad total de bloques libres en el gestor de memoria
int brute_force_count_free(MemoryManager* mm);

// Imprime en consola el estado de todos los bloques de memoria
void brute_force_print_all(MemoryManager* mm);


// ============================================================================
// 2. GREEDY / ALGORITMOS VORACES (greedy.c)
// ============================================================================

// Coordina la asignación de memoria llamando a First Fit, Best Fit o Worst Fit
int greedy_allocate(MemoryManager* mm, int pid, int size, int politica);

// Selecciona el proceso listo (READY) con menor burst_time (Shortest Job First)
int greedy_shortest_job(Process* processes, int n);

// Busca el bloque libre que genere el menor desperdicio de espacio (Best Fit)
MemoryBlock* greedy_best_block(MemoryManager* mm, int size);

// Busca el bloque libre con el mayor tamaño disponible (Worst Fit)
MemoryBlock* greedy_worst_block(MemoryManager* mm, int size);


// ============================================================================
// 3. BACKTRACKING (backtraking.c)
// ============================================================================

// Definición de la estructura para almacenar las capturas de estado de memoria
typedef struct {
    int *starts;
    int *sizes;
    int *frees;
    int *pids;
    int  count;
} MemorySnapshot;

// Toma una captura completa del estado actual de la memoria antes de una operación
MemorySnapshot* backtrack_save(const MemoryManager* mm);

// Restaura la memoria al estado exacto contenido en el snapshot
void backtrack_restore(MemoryManager* mm, const MemorySnapshot* snap);

// Libera la memoria dinámica asignada para el snapshot
void backtrack_free_snapshot(MemorySnapshot* snap);

// Imprime el contenido completo mapeado dentro de la captura (Snapshot)
void backtrack_print_snapshot(const MemorySnapshot* snap);


// ============================================================================
// 4. DIVIDE Y VENCERÁS (divide_conquer.c)
// ============================================================================

// Reorganiza los bloques moviendo los ocupados al inicio y unificando el espacio libre al final
void divide_conquer_compact(MemoryManager* mm);


// ============================================================================
// 5. PROGRAMACIÓN DINÁMICA (dp_topdown.c y dp_bottomup.c)
// ============================================================================

// Enfoque descendente (Memoización): Evalúa si los procesos caben en la memoria
int dp_topdown(int *sizes, int n, int target, int *memo);

// Enfoque ascendente (Tabulación): Resuelve de manera iterativa llenando la tabla
int dp_bottomup(int* sizes, int n, int target);

// Enfoque incremental: Optimiza el uso de memoria resolviendo con un arreglo unidimensional
int dp_incremental(int* sizes, int n, int target);


#endif // ALGORITHMS_H