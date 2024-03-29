#pragma once

#include "list.h"

typedef struct map_t {
    list_t *list;    
} map_t;

map_t *map_init(void);

void map_set(map_t *map, char *key, void *value);

void *map_get(map_t *map, char *key);

list_t *map_keys(map_t *map);

void map_free(map_t *map);
