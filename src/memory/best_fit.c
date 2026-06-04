/* * ==============================================================================
 * AVISO PARA SCHEDULER: 
 * Por diseño de la plantilla, esta función NO recibe el PID, solo el tamaño.
 * Retorna la posición 'start' del bloque asignado (o -1 si no hay espacio).
 * * ¡OBLIGATORIO!: Después de llamar a esta función con éxito, DEBES buscar 
 * el bloque en la lista y asignarle el 'pid' manualmente (bloque->pid = tu_pid).
 * ==============================================================================
 */
int mm_allocate_best_fit(
    MemoryManager* mm,
    int size
) {
    if (mm == NULL || mm->head == NULL || size <= 0) return -1;

    MemoryBlock* bloque = mm->head;
    MemoryBlock* mejor_bloque = NULL;
    int menor_diferencia = -1;

    // 1. Fase de búsqueda: Recorremos TODA la lista buscando al mejor candidato
    while (bloque != NULL) {
        if (bloque->free == 1 && bloque->size >= size) {
            int diferencia = bloque->size - size;
            
            // Si es el primer bloque que encontramos o si deja menos desperdicio que el anterior
            if (mejor_bloque == NULL || diferencia < menor_diferencia) {
                mejor_bloque = bloque;
                menor_diferencia = diferencia;
            }
        }
        bloque = bloque->next;
    }

    // 2. Fase de asignación: Si encontramos un ganador, lo partimos y asignamos
    if (mejor_bloque != NULL) {
        if (mejor_bloque->size > size) {
            MemoryBlock* blosig = (MemoryBlock*)malloc(sizeof(MemoryBlock));
            if (blosig == NULL) return -1; 

            blosig->start = mejor_bloque->start + size;
            blosig->size = mejor_bloque->size - size;
            blosig->free = 1;
            blosig->pid = -1;

            blosig->next = mejor_bloque->next;
            blosig->prev = mejor_bloque;

            if (mejor_bloque->next != NULL) {
                mejor_bloque->next->prev = blosig;
            }
            mejor_bloque->next = blosig;
        }

        mejor_bloque->size = size;
        mejor_bloque->free = 0;
        
        return mejor_bloque->start;
    }

    return -1; // No se encontró ningún bloque donde cupiera
}