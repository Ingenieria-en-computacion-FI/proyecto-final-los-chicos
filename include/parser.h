#ifndef PARSER_H
#define PARSER_H

#include "process.h" 

// Carga los procesos desde un archivo CSV y devuelve un arreglo dinámico
Process *parser_load_csv(const char *filepath, int *count);

// Imprime en consola la tabla con los procesos cargados
void parser_print_processes(const Process *processes, int count);

#endif // PARSER_H