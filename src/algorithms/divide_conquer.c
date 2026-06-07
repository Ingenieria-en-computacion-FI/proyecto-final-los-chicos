#include <stdio.h>
#include <stdlib.h>
#include "algorithms.h"

/*
 * Complejidad espacial del archivo: O(n)
 * donde n es el número de bloques de memoria.
 * Se usa un arreglo auxiliar para ordenar los bloques.
 *
 * DIVIDE Y VENCERÁS: Divide el problema en subproblemas más pequeños,
 * los resuelve de forma independiente y combina los resultados.
 * Aquí se aplica para reorganizar y compactar la memoria:
 * 1. Divide los bloques en ocupados y libres
 * 2. Ordena los ocupados por dirección de inicio
 * 3. Los combina al inicio dejando los libres al final
 */

// Merge de dos mitades ordenadas por start (parte del merge sort)
// Complejidad temporal: O(n) | Complejidad espacial: O(n)
static void merge(MemoryBlock** arr, int left, int mid, int right) {
    int n1 = mid - left + 1;
    int n2 = right - mid;

    MemoryBlock** L = malloc(n1 * sizeof(MemoryBlock*));
    MemoryBlock** R = malloc(n2 * sizeof(MemoryBlock*));
    if (!L || !R) { free(L); free(R); return; }

    for (int i = 0; i < n1; i++) L[i] = arr[left + i];
    for (int j = 0; j < n2; j++) R[j] = arr[mid + 1 + j];

    int i = 0, j = 0, k = left;
    while (i < n1 && j < n2) {
        if (L[i]->start <= R[j]->start) arr[k++] = L[i++];
        else                             arr[k++] = R[j++];
    }
    while (i < n1) arr[k++] = L[i++];
    while (j < n2) arr[k++] = R[j++];

    free(L);
    free(R);
}

// Merge sort recursivo sobre arreglo de punteros a bloques
// Complejidad temporal: O(n log n) | Complejidad espacial: O(n)
static void merge_sort(MemoryBlock** arr, int left, int right) {
    if (left >= right) return;
    int mid = left + (right - left) / 2;
    merge_sort(arr, left, mid);
    merge_sort(arr, mid + 1, right);
    merge(arr, left, mid, right);
}

// Compacta la memoria usando divide y vencerás:
// separa ocupados y libres, ordena ocupados por start,
// los mueve al inicio y crea un bloque libre al final
// Complejidad temporal: O(n log n) | Complejidad espacial: O(n)
void divide_conquer_compact(MemoryManager* mm) {
    if (!mm || !mm->head) return;

    // Contar bloques ocupados
    int count = 0;
    for (MemoryBlock* cur = mm->head; cur; cur = cur->next)
        if (!cur->free) count++;

    if (count == 0) return;

    // Recolectar bloques ocupados en arreglo
    MemoryBlock** ocupados = malloc(count * sizeof(MemoryBlock*));
    if (!ocupados) return;

    int libre_total = 0;
    int idx = 0;
    for (MemoryBlock* cur = mm->head; cur; cur = cur->next) {
        if (!cur->free) ocupados[idx++] = cur;
        else            libre_total += cur->size;
    }

    // Ordenar ocupados por dirección de inicio con merge sort
    merge_sort(ocupados, 0, count - 1);

    // Reasignar direcciones de inicio consecutivas
    int ubiactual = 0;
    for (int i = 0; i < count; i++) {
        ocupados[i]->start = ubiactual;
        ubiactual += ocupados[i]->size;
    }

    // Reconstruir la lista: ocupados primero, un bloque libre al final
    mm->head = ocupados[0];
    ocupados[0]->prev = NULL;
    for (int i = 0; i < count - 1; i++) {
        ocupados[i]->next = ocupados[i+1];
        ocupados[i+1]->prev = ocupados[i];
    }

    // Agregar bloque libre al final si hay memoria libre
    if (libre_total > 0) {
        MemoryBlock* libre = malloc(sizeof(MemoryBlock));
        if (libre) {
            libre->start = ubiactual;
            libre->size  = libre_total;
            libre->free  = 1;
            libre->pid   = -1;
            libre->next  = NULL;
            libre->prev  = ocupados[count - 1];
            ocupados[count - 1]->next = libre;
        }
    } else {
        ocupados[count - 1]->next = NULL;
    }

    free(ocupados);
    printf("Compactacion completada: %d bloques ocupados, %d libres al final\n",
           count, libre_total);
}