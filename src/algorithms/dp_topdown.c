#include <stdio.h>
#include <stdlib.h>
#include "algorithms.h"

/*
 * Complejidad espacial del archivo: O(n * target)
 * PROGRAMACIÓN DINÁMICA DESCENDENTE (Memoización)
 * Complejidad temporal: O(n * target) | Complejidad espacial: O(n * target)
 */
 
 // Función recursiva con memoización | O(n * target)
static int helper(int* sizes, int i, int cap, int* memo, int target) {
    if (cap == 0) return 1;
    if (i == 0 || cap < 0) return 0;
    int idx = i * (target + 1) + cap;
    if (memo[idx] != -1) return memo[idx];
    memo[idx] = helper(sizes, i-1, cap, memo, target) ||
                (sizes[i-1] <= cap ? helper(sizes, i-1, cap - sizes[i-1], memo, target) : 0);
    return memo[idx];
}
 
// Inicializa memo y resuelve el problema descendentemente | O(n * target)
int dp_topdown(int* sizes, int n, int target, int* memo) {
    if (!sizes || n <= 0 || target <= 0) return 0;
    for (int i = 0; i <= n * (target + 1); i++) memo[i] = -1;
    int res = helper(sizes, n, target, memo, target);
    printf("DP TopDown: %s asignar %d unidades con %d procesos\n",
           res ? "SI es posible" : "NO es posible", target, n);
    return res;
}