#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "process.h"

/*
 * parser.c  –  src/utils/parser.c
 *
 * Lee archivos CSV generados por Python (generate_processes.py)
 * y llena un arreglo de Process para pasarlo a los schedulers.
 *
 * Formato esperado del CSV:
 *   pid,burst_time,priority,memory_required
 *   1,5,2,128
 *   2,3,1,64
 *
 * Complejidad temporal: O(n)  donde n = número de líneas
 * Complejidad espacial: O(1)  (escribe directo en el arreglo)
 */

#define PARSER_MAX_LINE 256

/*
 * parse_processes_from_csv
 * Llena el arreglo `out` con los procesos leídos del CSV.
 * Devuelve el número de procesos leídos, o -1 si hay error.
 * `max` es el tamaño máximo del arreglo `out`.
 */
int parse_processes_from_csv(const char* filepath, Process* out, int max) {
    if (!filepath || !out || max <= 0) return -1;

    FILE* f = fopen(filepath, "r");
    if (!f) {
        fprintf(stderr, "[PARSER] Error: no se pudo abrir '%s'\n", filepath);
        return -1;
    }

    char line[PARSER_MAX_LINE];
    int count = 0;

    while (fgets(line, sizeof(line), f)) {
        /* Saltar encabezado y líneas vacías */
        if (line[0] == '\n' || line[0] == '#') continue;
        if (line[0] == 'p'  || line[0] == 'P') continue;

        if (count >= max) {
            fprintf(stderr, "[PARSER] Capacidad máxima (%d) alcanzada\n", max);
            break;
        }

        int pid, burst_time, priority, memory_required;
        if (sscanf(line, "%d,%d,%d,%d",
                   &pid, &burst_time, &priority, &memory_required) != 4) {
            fprintf(stderr, "[PARSER] Línea malformada, se omite: %s", line);
            continue;
        }

        out[count].pid              = pid;
        out[count].burst_time       = burst_time;
        out[count].remaining_time   = burst_time;
        out[count].priority         = priority;
        out[count].memory_required  = memory_required;
        out[count].state            = READY;
        count++;
    }

    fclose(f);
    printf("[PARSER] %d procesos cargados desde '%s'\n", count, filepath);
    return count;
}

/*
 * parse_quantum_from_file
 * Lee un entero desde un archivo de una sola línea.
 * Devuelve el quantum leído, o default_val si falla.
 */
int parse_quantum_from_file(const char* filepath, int default_val) {
    if (!filepath) return default_val;

    FILE* f = fopen(filepath, "r");
    if (!f) {
        fprintf(stderr, "[PARSER] No se encontró config de quantum, "
                        "usando default=%d\n", default_val);
        return default_val;
    }

    int quantum = default_val;
    if (fscanf(f, "%d", &quantum) != 1 || quantum <= 0) {
        fprintf(stderr, "[PARSER] Quantum inválido, usando default=%d\n",
                default_val);
        quantum = default_val;
    }

    fclose(f);
    printf("[PARSER] Quantum=%d cargado desde '%s'\n", quantum, filepath);
    return quantum;
}
