#pragma once

#include "list.h"
#include "map.h"

typedef struct graph_t {
    map_t *adj;    
} graph_t;

graph_t* graph_init(void);

void graph_add_edge(graph_t *graph, char *u, char *v);

list_t* graph_get_children(graph_t *graph, char *u);

list_t* graph_topo_order(graph_t *graph);

void graph_free(graph_t *graph);

void graph_print(graph_t *graph);
