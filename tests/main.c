#include <stdio.h>
#include <stdlib.h>
#include "process.h"
#include "scheduler.h"
#include "memory_manager.h"
#include "algorithms.h"
#include "parser.h"
#include "timer.h"

/*
 * Complejidad espacial de main.c: O(n * target)
 * donde n = número de procesos, target = MEMORY_TOTAL / 2
 *
 * Desglose:
 *   processes  → O(n)
 *   sizes      → O(n)
 *   memo       → O(n * target)  ← dominante
 *   mm/mm2/mm3 → O(n) cada uno
 *   snap       → O(n)
 *   finished   → O(n)
 *   variables  → O(1)
 */

#define MEMORY_TOTAL 4096
#define QUANTUM      2

int main(int argc, char* argv[]) {

    // 1. Cargar procesos
    int n = 0;
    Process* processes = NULL;

    if (argc > 1) {
        processes = parser_load_csv(argv[1], &n);
        if (!processes || n == 0) {
            fprintf(stderr, "Error cargando %s\n", argv[1]);
            return 1;
        }
    } else {
        n = 5;
        processes = malloc(n * sizeof(Process));
        processes[0] = process_create(1,  8, 2, 200);
        processes[1] = process_create(2,  4, 1, 100);
        processes[2] = process_create(3, 10, 3, 300);
        processes[3] = process_create(4,  2, 1,  50);
        processes[4] = process_create(5,  6, 2, 150);
    }

    printf("\n=== Mini OS Simulator ===\n");
    printf("Procesos: %d | Memoria: %d | Quantum: %d\n\n", n, MEMORY_TOTAL, QUANTUM);

    // 2. Memoria — probar los 3 algoritmos de asignacion (greedy)
    MemoryManager* mm = mm_create(MEMORY_TOTAL);

    printf("--- Asignacion First Fit ---\n");
    for (int i = 0; i < n; i++) {
        int r = greedy_allocate(mm, processes[i].pid, processes[i].memory_required, 0);
        printf("PID %d: %s\n", processes[i].pid, r ? "asignado" : "sin espacio");
    }

    printf("\n--- Asignacion Best Fit (snapshot) ---\n");
    MemorySnapshot* snap = backtrack_save(mm);   // guardar estado antes
    MemoryManager* mm2 = mm_create(MEMORY_TOTAL);
    for (int i = 0; i < n; i++) {
        int r = greedy_allocate(mm2, processes[i].pid, processes[i].memory_required, 1);
        printf("PID %d: %s\n", processes[i].pid, r ? "asignado" : "sin espacio");
    }
    mm_destroy(mm2);

    printf("\n--- Asignacion Worst Fit ---\n");
    MemoryManager* mm3 = mm_create(MEMORY_TOTAL);
    for (int i = 0; i < n; i++) {
        int r = greedy_allocate(mm3, processes[i].pid, processes[i].memory_required, 2);
        printf("PID %d: %s\n", processes[i].pid, r ? "asignado" : "sin espacio");
    }
    mm_destroy(mm3);

    // 3. Fragmentacion y fuerza bruta
    printf("\nFragmentacion: %s | Bloques libres: %d\n",
           brute_force_check_fragmentation(mm, 500) ? "SI" : "NO",
           brute_force_count_free(mm));

    // 4. Backtracking — restaurar estado antes de best fit
    printf("\n--- Backtracking: restaurando estado inicial ---\n");
    backtrack_restore(mm, snap);
    backtrack_free_snapshot(snap);
    printf("Estado restaurado\n");

    // 5. Programacion dinamica
    printf("\n--- Programacion Dinamica ---\n");
    int sizes[n];
    for (int i = 0; i < n; i++) sizes[i] = processes[i].memory_required;
    int target = MEMORY_TOTAL / 2;
    int* memo = malloc((n + 1) * (target + 1) * sizeof(int));

    dp_topdown(sizes, n, target, memo);
    dp_bottomup(sizes, n, target);
    dp_incremental(sizes, n, target);
    free(memo);

    // 6. Schedulers
    Timer t;
    LinkedList* finished;

    printf("\n--- FIFO ---\n");
    finished = list_create();
    timer_start(&t);
    SchedulerResult fifo = scheduler_fifo(processes, n, finished);
    timer_stop(&t);
    scheduler_print_result(&fifo, "FIFO");
    timer_print(&t, "FIFO");
    list_destroy(finished);

    printf("\n--- Round Robin (quantum=%d) ---\n", QUANTUM);
    finished = list_create();
    timer_start(&t);
    SchedulerResult rr = scheduler_round_robin(processes, n, QUANTUM, finished);
    timer_stop(&t);
    scheduler_print_result(&rr, "Round Robin");
    timer_print(&t, "Round Robin");
    list_destroy(finished);

    printf("\n--- SJF ---\n");
    finished = list_create();
    timer_start(&t);
    SchedulerResult sjf = scheduler_sjf(processes, n, finished);
    timer_stop(&t);
    scheduler_print_result(&sjf, "SJF");
    timer_print(&t, "SJF");
    list_destroy(finished);

    // 7. Compactacion con divide y venceras
    printf("\n--- Compactacion (Divide y Venceras) ---\n");
    divide_conquer_compact(mm);
    brute_force_print_all(mm);

    // 8. Liberar
    mm_destroy(mm);
    free(processes);

    printf("\n=== Simulacion completada ===\n");
    return 0;
}