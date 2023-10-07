#include<stdlib.h>

#include "list.h"

node_t* node_init(void *val, node_t *prev, node_t *next) {
    node_t *node = (node_t*) malloc(sizeof(node_t));
    node->val = val;
    node->prev = prev;
    node->next = next;
    return node;
}

list_t* list_init() {
    list_t *list = (list_t*) malloc(sizeof(list_t));
    list->head = NULL;
    list->tail = NULL;
    list->size = 0;
    return list;
}

void list_push(list_t *list, void *val) {
    node_t *new_node = node_init(val, list->tail, NULL);
    if (list->size == 0) {
        list->tail = new_node;
        list->head = new_node;
    } else {
        list->tail->next = new_node;
        list->tail = new_node;
    }
    list->size++;
}

void list_pop(list_t *list) {
    if (list->size == 0) return;
    if (list->size == 1) {
        list->head = NULL;
        list->tail = NULL;
    } else {
        list->tail = list->tail->prev;
    }
    list->size--;
}

size_t list_size(list_t *list) {
    return list->size;
}

list_itr_t* list_itr_init(list_t *list) {
    list_itr_t *itr = (list_itr_t*) malloc(sizeof(list_itr_t));
    itr->cur = list->head;
    return itr;
}

int list_itr_has_next(list_itr_t *itr) {
    return itr->cur != NULL;
}

void* list_itr_next(list_itr_t *itr) {
    void *ret = itr->cur->val;
    itr->cur = itr->cur->next;
    return ret;
}
