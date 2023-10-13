#include<stdlib.h>
#include<string.h>

#include "map.h"
#include "list.h"

typedef struct pair_t {
    char *key;
    void *value;
} pair_t;

pair_t* pair_init(char *key, void *value) {
    pair_t *pair = (pair_t*) malloc(sizeof(pair_t));
    pair->key = key;
    pair->value = value;
    return pair;
}

map_t *map_init() {
    map_t *map = (map_t*) malloc(sizeof(map_t));
    map->list = list_init();
    return map;
}

void map_set(map_t *map, char *key, void *value) {
    list_itr_t *it = list_itr_init(map->list);
    while (list_itr_has_next(it)) {
        pair_t *pair = (pair_t*) list_itr_next(it);
        if (strcmp(pair->key, key) == 0) {
            pair->value = value;
            return;
        }
    }
    list_push_back(map->list, pair_init(key, value));
}

void* map_get(map_t *map, char *key) {
    list_itr_t *it = list_itr_init(map->list);
    while (list_itr_has_next(it)) {
        pair_t *pair = (pair_t*) list_itr_next(it);
        if (strcmp(pair->key, key) == 0) {
            return pair->value;
        }
    }
    return NULL;
}
