#include <stdio.h>
#include <stdlib.h>
#include "scheduler.h"

/*
 * Complejidad espacial del archivo: O(n)
 * Complejidad temporal general: O(n)
 * Cada proceso entra y sale de la cola exactamente una vez.
 */

// Ejecuta FIFO: el primero en llegar, primero en ejecutarse | O(n)

/* ---- Mocks de memoria ----------------------------------------
 * Simulan asignación y liberación de memoria mientras el módulo
 * real no esté disponible.
 * Cuando esté listo, solo se reemplazan estas dos funciones.
 * -------------------------------------------------------------- */

SchedulerResult scheduler_fifo(Process* processes, int n, LinkedList* finished) {
    SchedulerResult result = {0, 0.0f, 0.0f, 0};
    Queue* q = queue_create();
    Stack* historial = stack_create();
    int* wait = calloc(n, sizeof(int));
    int tiempo = 0;

    for (int i = 0; i < n; i++) {
        processes[i].state = READY;
        processes[i].remaining_time = processes[i].burst_time;
        queue_enqueue(q, processes[i]);
    }

    while (!queue_is_empty(q)) {
        Process p;
        queue_dequeue(q, &p);
        p.state = RUNNING;
        wait[p.pid - 1] = tiempo;
        stack_push(historial, p.pid);
        printf("Tiempo %d: PID %d ejecuta %d unidades\n", tiempo, p.pid, p.burst_time);
        tiempo += p.burst_time;
        p.remaining_time = 0;
        p.state = FINISHED;
        if (finished) list_insert_sorted(finished, p);
    }

    result.total_processes = n;
    result.total_time = tiempo;
    for (int i = 0; i < n; i++) {
        result.avg_waiting_time += wait[i];
        result.avg_turnaround_time += wait[i] + processes[i].burst_time;
    }
    result.avg_waiting_time /= n;
    result.avg_turnaround_time /= n;

    free(wait);
    queue_destroy(q);
    stack_destroy(historial);
    return result;
}
