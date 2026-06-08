#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "process.h"
 
#define MAX_LINE 256
 
static ProcessState parse_state(const char *str) {
    if (strcmp(str, "READY")    == 0) return READY;
    if (strcmp(str, "RUNNING")  == 0) return RUNNING;
    if (strcmp(str, "BLOCKED")  == 0) return BLOCKED;
    if (strcmp(str, "FINISHED") == 0) return FINISHED;
    return READY;
}
 
Process *parser_load_csv(const char *filepath, int *count) {
    if (!filepath || !count) return NULL;
 
    FILE *f = fopen(filepath, "r");
    if (!f) {
        fprintf(stderr, "Error: no se pudo abrir %s\n", filepath);
        return NULL;
    }
 
    char line[MAX_LINE];
    int  total = 0;
 
    fgets(line, sizeof(line), f);
 
    while (fgets(line, sizeof(line), f))
        total++;
 
    if (total == 0) {
        fclose(f);
        fprintf(stderr, "Error: el archivo esta vacio\n");
        return NULL;
    }
 
    rewind(f);
    fgets(line, sizeof(line), f);
 
    Process *processes = malloc(total * sizeof(Process));
    if (!processes) {
        fclose(f);
        return NULL;
    }
 
    int i = 0;
    while (fgets(line, sizeof(line), f)) {
        line[strcspn(line, "\n")] = '\0';
 
        char state_str[16];
        sscanf(line, "%d,%d,%d,%d,%d,%15s",
            &processes[i].pid,
            &processes[i].burst_time,
            &processes[i].remaining_time,
            &processes[i].priority,
            &processes[i].memory_required,
            state_str);
 
        processes[i].state = parse_state(state_str);
        i++;
    }
 
    fclose(f);
    *count = total;
    return processes;
}
 
void parser_print_processes(const Process *processes, int count) {
    if (!processes || count <= 0) return;
    printf("%-6s %-12s %-16s %-10s %-18s %-10s\n",
           "PID", "BURST_TIME", "REMAINING_TIME", "PRIORITY", "MEMORY_REQUIRED", "STATE");
    printf("----------------------------------------------------------------------\n");
    for (int i = 0; i < count; i++) {
        printf("%-6d %-12d %-16d %-10d %-18d %-10s\n",
               processes[i].pid,
               processes[i].burst_time,
               processes[i].remaining_time,
               processes[i].priority,
               processes[i].memory_required,
               process_state_str(processes[i].state));
    }
}