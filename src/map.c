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

map_t *map_init(void) {
    map_t *map = (map_t*) malloc(sizeof(map_t));
    map->list = list_init();
    return map;
}

void map_set(map_t *map, char *key, void *value) {
    list_itr_t *it = list_itr_init(map->list);
    int found = 0;
    while (list_itr_has_next(it)) {
        pair_t *pair = (pair_t*) list_itr_next(it);
        if (strcmp(pair->key, key) == 0) {
            pair->value = value;
            found = 1;
            break;
        }
    }
    if (!found) {
        list_push_back(map->list, pair_init(key, value));
    }
    list_itr_free(it);
}

void* map_get(map_t *map, char *key) {
    list_itr_t *it = list_itr_init(map->list);
    void* ret = NULL;
    while (list_itr_has_next(it)) {
        pair_t *pair = (pair_t*) list_itr_next(it);
        if (strcmp(pair->key, key) == 0) {
            ret = pair->value;
            break;
        }
    }
    list_itr_free(it);
    return ret;
}

list_t *map_keys(map_t *map) {
    list_t *keys = list_init();
    list_itr_t *it = list_itr_init(map->list);
    while (list_itr_has_next(it)) {
        pair_t *pair = (pair_t*) list_itr_next(it);
        list_push_back(keys, pair->key);
    }
    list_itr_free(it);
    return keys;
}

void map_free(map_t *map) {
    if (map == NULL) return;
    list_itr_t *itr = list_itr_init(map->list);
    while (list_itr_has_next(itr)) {
        pair_t *p = list_itr_next(itr);
        free(p);
    }
    list_itr_free(itr);
    list_free(map->list);
    free(map);
}
