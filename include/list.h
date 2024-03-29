#pragma once

#include "e4c_lite.h"


E4C_DECLARE_EXCEPTION(ListException);
E4C_DECLARE_EXCEPTION(EmptyListException);

typedef struct node_t {
    void *val;
    struct node_t *prev;
    struct node_t *next;
} node_t;

typedef struct list_t {
    node_t *head;
    node_t *tail;
    size_t size;
} list_t;

typedef struct list_itr_t {
    node_t *cur;
} list_itr_t;

node_t* node_init(void *val, node_t *next, node_t* prev);

list_t* list_init(void);

void list_push_back(list_t *list, void *val);

void list_pop_back(list_t *list);

void list_push_front(list_t *list, void *val);

void list_pop_front(list_t *list);

size_t list_size(list_t *list);

int list_is_empty(list_t *list);

void list_free(list_t *list);

list_itr_t* list_itr_init(list_t *list);

int list_itr_has_next(list_itr_t *itr);

void* list_itr_next(list_itr_t *itr);

void list_itr_free(list_itr_t *itr);
