#ifndef LINKED_LIST_H
#define LINKED_LIST_H
 
#include "process.h"
 
typedef struct ListNode {
    Process process;
    struct ListNode *next;
} ListNode;
 
// Lista ligada simple
typedef struct {
    ListNode *head;
    int size;
} LinkedList;
 
LinkedList *list_create(void);
void        list_destroy(LinkedList *list);
 
/* Inserta manteniendo orden ascendente por PID */
int  list_insert_sorted(LinkedList *list, Process p);
 
int  list_find(const LinkedList *list, int pid, Process *out);
int  list_remove(LinkedList *list, int pid);
 
int  list_is_empty(const LinkedList *list);
int  list_size(const LinkedList *list);
void list_print(const LinkedList *list);
 
#endif 