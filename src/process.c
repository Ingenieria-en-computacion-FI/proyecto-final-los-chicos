#include <stdio.h>
#include "process.h"

/*
 * Complejidad espacial del archivo: O(1)
 * Todas las funciones operan sobre un solo proceso.
 */

// Crea un proceso con valores iniciales y estado READY | O(1)
Process process_create(int pid, int burst_time, int priority, int memory_required) {
    Process p;
    p.pid             = pid;
    p.burst_time      = burst_time;
    p.remaining_time  = burst_time;
    p.priority        = priority;
    p.memory_required = memory_required;
    p.state           = READY;
    return p;
}

// Convierte ProcessState a string legible | O(1)
const char* process_state_str(ProcessState state) {
    switch (state) {
        case READY:    return "READY";
        case RUNNING:  return "RUNNING";
        case BLOCKED:  return "BLOCKED";
        case FINISHED: return "FINISHED";
        default:       return "UNKNOWN";
    }
}

// Imprime todos los campos del proceso | O(1)
void process_print(const Process* p) {
    if (!p) return;
    printf("PID:%d bt:%d rt:%d pri:%d mem:%d state:%s\n",
           p->pid, p->burst_time, p->remaining_time,
           p->priority, p->memory_required,
           process_state_str(p->state));
}