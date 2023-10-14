#include<stdlib.h>

#include "list.h"


E4C_DEFINE_EXCEPTION(ListException, "List Exception!", RuntimeException);
E4C_DEFINE_EXCEPTION(EmptyListException, "The list is empty!", ListException);

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

void list_push_back(list_t *list, void *val) {
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

void list_pop_back(list_t *list) {
    if (list->size == 0) {
        E4C_THROW(EmptyListException, NULL);
    }
    if (list->size == 1) {
        list->head = NULL;
        list->tail = NULL;
    } else {
        list->tail = list->tail->prev;
    }
    list->size--;
}

void list_push_front(list_t *list, void *val) {
    node_t *new_node = node_init(val, NULL, list->head);
    if (list->size == 0) {
        list->tail = new_node;
        list->head = new_node;
    } else {
        list->head->prev = new_node;
        list->head = new_node;
    }
    list->size++;
}

void list_pop_front(list_t *list) {
    if (list->size == 0) {
        E4C_THROW(EmptyListException, NULL);
    }
    if (list->size == 1) {
        list->head = NULL;
        list->tail = NULL;
    } else {
        list->head = list->head->next;
    }
    list->size--;
}

size_t list_size(list_t *list) {
    return list->size;
}

int list_is_empty(list_t *list) {
    return list->size == 0;
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
