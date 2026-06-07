#include <stdio.h>
#include <stdlib.h>
#include "scheduler.h"

/*
 * Complejidad espacial del archivo: O(n)
 * Complejidad temporal general: O(n * ceil(burst_time / quantum))
 * Cada proceso puede entrar y salir de la cola varias veces.
 */

// Ejecuta Round Robin con quantum dado | O(n * burst_time/quantum)

SchedulerResult scheduler_round_robin(Process* processes, int n, int quantum, LinkedList* finished) {
    SchedulerResult result = {0, 0.0f, 0.0f, 0};
    CircularQueue* cq = cqueue_create();
    Stack* historial  = stack_create();
    int* wait = calloc(n, sizeof(int));
    int* turnaround = calloc(n, sizeof(int));
    int tiempo = 0;

    for (int i = 0; i < n; i++) {
        processes[i].state = READY;
        processes[i].remaining_time = processes[i].burst_time;
        cqueue_enqueue(cq, processes[i]);
    }

    while (!cqueue_is_empty(cq)) {
        Process p;
        cqueue_dequeue(cq, &p);
        int ejecutado = (p.remaining_time > quantum) ? quantum : p.remaining_time;
        p.state = RUNNING;
        p.remaining_time -= ejecutado;
        tiempo += ejecutado;
        stack_push(historial, p.pid);
        printf("Tiempo %d: PID %d ejecuta %d unidades, restante: %d\n",
               tiempo, p.pid, ejecutado, p.remaining_time);

        if (p.remaining_time == 0) {
            p.state = FINISHED;
            turnaround[p.pid-1] = tiempo;
            wait[p.pid-1] = tiempo - p.burst_time;
            if (finished) list_insert_sorted(finished, p);
        } else {
            p.state = READY;
            cqueue_enqueue(cq, p);
        }
    }

    result.total_processes = n;
    result.total_time = tiempo;
    for (int i = 0; i < n; i++) {
        result.avg_waiting_time += wait[i];
        result.avg_turnaround_time += turnaround[i];
    }
    result.avg_waiting_time /= n;
    result.avg_turnaround_time /= n;

    free(wait);
    free(turnaround);
    cqueue_destroy(cq);
    stack_destroy(historial);
    return result;
}