#include <stdio.h>
#include <stdlib.h>
#include "scheduler.h"

/*
 * Complejidad espacial del archivo: O(n)
 * Complejidad temporal general: O(n^2)
 * El ordenamiento por selección es O(n^2),
 * la ejecución de cada proceso es O(1).
 */

/* ---- Mocks de memoria ----------------------------------------
 * Simulan asignación y liberación de memoria mientras el módulo
 * real del Integrante 2 no esté disponible.
 * Cuando esté listo, solo se reemplazan estas dos funciones.
 * -------------------------------------------------------------- */
 
// Ordena procesos por burst_time ascendente | O(n^2)
static void ordenar_burst(Process* arr, int n) {
    for (int i = 0; i < n - 1; i++) {
        int min = i;
        for (int j = i + 1; j < n; j++)
            if (arr[j].burst_time < arr[min].burst_time) min = j;
        if (min != i) {
            Process tmp = arr[i];
            arr[i] = arr[min];
            arr[min] = tmp;
        }
    }
}

// Ejecuta SJF: el proceso más corto se ejecuta primero | O(n^2)
SchedulerResult scheduler_sjf(Process* processes, int n, LinkedList* finished) {
    SchedulerResult result = {0, 0.0f, 0.0f, 0};
    Stack* historial = stack_create();
    Process* copia = malloc(n * sizeof(Process));
    int* wait = calloc(n, sizeof(int));
    int* turnaround = calloc(n, sizeof(int));
    int tiempo = 0;

    for (int i = 0; i < n; i++) {
        copia[i] = processes[i];
        copia[i].state = READY;
        copia[i].remaining_time = copia[i].burst_time;
    }
    ordenar_burst(copia, n);

    for (int i = 0; i < n; i++) {
        copia[i].state = RUNNING;
        wait[i] = tiempo;
        stack_push(historial, copia[i].pid);
        printf("Tiempo %d: PID %d ejecuta %d unidades\n", tiempo, copia[i].pid, copia[i].burst_time);
        tiempo += copia[i].burst_time;
        turnaround[i] = tiempo;
        copia[i].remaining_time = 0;
        copia[i].state = FINISHED;
        if (finished) list_insert_sorted(finished, copia[i]);
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
    free(copia);
    stack_destroy(historial);
    return result;
}
