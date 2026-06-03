#ifndef ALGORITHMS_H
#define ALGORITHMS_H

#include "memory_manager.h"
#include "process.h"

// Fuerza bruta
MemoryBlock *brute_force_find_block(MemoryManager *mm, int required);

// Greedy
MemoryBlock *first_fit(MemoryManager *mm, int required);

MemoryBlock *best_fit(MemoryManager *mm, int required);

MemoryBlock *worst_fit(MemoryManager *mm, int required);

// Backtraking
typedef struct {
    int *starts;
    int *sizes;
    int *frees;
    int *pids;
    int  count;
} MemorySnapshot;

MemorySnapshot *backtrack_save(const MemoryManager *mm);
void backtrack_restore(MemoryManager *mm, const MemorySnapshot *snap);
void backtrack_free_snapshot(MemorySnapshot *snap);

// Divide y venceras

void divide_conquer_compact(MemoryManager *mm);

// Programación dinamica

int dp_topdown(int *sizes, int n, int target, int *memo);  // M
int dp_bottomup(int *sizes, int n, int target);            // T
int dp_incremental(int *sizes, int n, int target);         // I

#endif