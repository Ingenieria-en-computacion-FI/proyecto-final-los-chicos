#include <stdio.h>
#include <stdlib.h>
#include "linked_list.h"

//   Crea una lista vacía en el heap: Complejidad: O(1)

LinkedList *list_create(void) {
    LinkedList *list = (LinkedList *)malloc(sizeof(LinkedList));
    if (!list) {
        fprintf(stderr, "Error: no se pudo crear la lista\n");
        return NULL;
    }
    list->head = NULL;
    list->size = 0;
    return list;
}
 //  Inserta un proceso manteniendo orden ascendente por PID: Complejidad: O(n)

int list_insert_sorted(LinkedList *list, Process p) {
    if (!list) return 0;

    ListNode *node = (ListNode *)malloc(sizeof(ListNode));
    if (!node) {
        fprintf(stderr, "Error: no se pudo insertar en la lista\n");
        return 0;
    }
    node->process = p;
    node->next    = NULL;

    // Caso 1: lista vacía.
    if (!list->head || list->head->process.pid >= p.pid) {
        node->next  = list->head;
        list->head  = node;
        list->size++;
        return 1;
    }

    // Caso 2: buscar la posición correcta 
    ListNode *cur = list->head;
    while (cur->next && cur->next->process.pid < p.pid) {
        cur = cur->next;
    }
    node->next = cur->next;
    cur->next  = node;
    list->size++;
    return 1;
}

//    Busca un proceso por PID y lo copia en *out, Devuelve 1 si lo encontró, 0 si no. Complejidad: O(n)

int list_find(const LinkedList *list, int pid, Process *out) {
    if (!list) return 0;
    ListNode *cur = list->head;
    while (cur) {
        if (cur->process.pid == pid) {
            *out = cur->process;
            return 1;
        }
        cur = cur->next;
    }
    return 0;
}


  // Elimina el nodo con el PID indicado
  // Devuelve 1 si lo eliminó, 0 si no lo encontró
  // Complejidad: O(n)

int list_remove(LinkedList *list, int pid) {
    if (!list || !list->head) return 0;

    /* Caso: es el primer nodo */
    if (list->head->process.pid == pid) {
        ListNode *tmp = list->head;
        list->head    = tmp->next;
        free(tmp);
        list->size--;
        return 1;
    }

    // Buscar el nodo anterior al que queremos eliminar 

    ListNode *cur = list->head;
    while (cur->next && cur->next->process.pid != pid) {
        cur = cur->next;
    }
    if (!cur->next) return 0;   // No encontrado

    ListNode *tmp = cur->next;
    cur->next = tmp->next;
    free(tmp);
    list->size--;
    return 1;
}

// Varificación de lista vacía

int list_is_empty(const LinkedList *list) {
    if (!list) return 1;
    return list->size == 0;
}

int list_size(const LinkedList *list) {
    if (!list) return 0;
    return list->size;
}

// Muestra todos los procesos en orden: Complejidad O(n)

void list_print(const LinkedList *list) {
    if (!list || !list->head) {
        printf("[Lista vacía]\n");
        return;
    }
    ListNode *cur = list->head;
    printf("HEAD -> ");
    while (cur) {
        printf("[PID:%d bt:%d st:%d] -> ",
               cur->process.pid,
               cur->process.burst_time,
               cur->process.state);
        cur = cur->next;
    }
    printf("NULL\n");
}

// Libera todos los nodos de la lista: Complejidad O(n)

void list_destroy(LinkedList *list) {
    if (!list) return;
    ListNode *cur = list->head;
    while (cur) {
        ListNode *tmp = cur;
        cur = cur->next;
        free(tmp);
    }
    free(list);
}