#include <stdio.h>
#include <stdlib.h>
#include "algorithms.h"

//  Complejidad espacial del archivo: O(n * target)
//  PROGRAMACIÓN DINÁMICA ASCENDENTE (Tabulación) e INCREMENTAL

// Llena tabla iterativamente de abajo hacia arriba | O(n * target)

int dp_bottomup(int* sizes, int n, int target) {
    if (!sizes || n <= 0 || target <= 0) return 0;

    int** dp = malloc((n + 1) * sizeof(int*));
    for (int i = 0; i <= n; i++) {
        dp[i] = calloc(target + 1, sizeof(int));
        dp[i][0] = 1;
    }

    for (int i = 1; i <= n; i++)
        for (int cap = 1; cap <= target; cap++) {
            dp[i][cap] = dp[i-1][cap];
            if (sizes[i-1] <= cap)
                dp[i][cap] = dp[i][cap] || dp[i-1][cap - sizes[i-1]];
        }

    int res = dp[n][target];
    printf("DP BottomUp: %s asignar %d unidades con %d procesos\n",
           res ? "SI es posible" : "NO es posible", target, n);

    for (int i = 0; i <= n; i++) free(dp[i]);
    free(dp);
    return res;
}

// Agrega procesos uno a uno con una sola fila — O(target) en espacio | O(n * target)
int dp_incremental(int* sizes, int n, int target) {
    if (!sizes || n <= 0 || target <= 0) return 0;

    int* dp = calloc(target + 1, sizeof(int));
    dp[0] = 1;

    for (int i = 0; i < n; i++)
        for (int cap = target; cap >= sizes[i]; cap--)
            dp[cap] = dp[cap] || dp[cap - sizes[i]];

    int res = dp[target];
    printf("DP Incremental: %s asignar %d unidades con %d procesos\n",
           res ? "SI es posible" : "NO es posible", target, n);

    free(dp);
    return res;
}