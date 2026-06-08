#include <stdio.h>
#include "scheduler.h"

// Imprime los resultados de un scheduler | O(1)
void scheduler_print_result(const SchedulerResult* r, const char* name) {
    if (!r || !name) return;
    printf("Resultado %s:\n", name);
    printf("  Procesos     : %d\n", r->total_processes);
    printf("  Tiempo total : %d\n", r->total_time);
    printf("  Espera prom  : %.2f\n", r->avg_waiting_time);
    printf("  Retorno prom : %.2f\n", r->avg_turnaround_time);
}

// Reinicia los procesos a su estado inicial | O(n)
void scheduler_reset_processes(Process* processes, int n) {
    if (!processes) return;
    for (int i = 0; i < n; i++) {
        processes[i].state = READY;
        processes[i].remaining_time = processes[i].burst_time;
    }
}