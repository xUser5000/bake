#include<stdlib.h>

#include "graph.h"
#include "list.h"
#include "map.h"

graph_t* graph_init() {
    graph_t *graph = malloc(sizeof(graph_t));
    graph->adj = map_init();
    return graph;
}

void graph_add_edge(graph_t *graph, char *u, char *v) {
    if (map_get(graph->adj, u) == NULL) {
        map_set(graph->adj, u, list_init());
    }
    list_t *children = map_get(graph->adj, u);
    list_push_back(children, v);
}

list_t* graph_get_children(graph_t *graph, char *u) {
    return map_get(graph->adj, u);
}

list_t* graph_topo_order(graph_t *graph) {
    map_t *indegree = map_init();
    list_t *queue = list_init();
    list_t *topo_order = list_init();

    /* Getting all nodes in the graph */
    list_t *nodes = map_keys(graph->adj);

    /* Setting the in-degree of each node */
    list_itr_t *nodes_itr = list_itr_init(nodes);
    while (list_itr_has_next(nodes_itr)) {
        char *u = (char*) list_itr_next(nodes_itr);
        list_t *children = graph_get_children(graph, u);

        /* For each child, increment its in-degree by one */
        list_itr_t *children_itr = list_itr_init(children);
        while (list_itr_has_next(children_itr)) {
            char *v = (char*) list_itr_next(children_itr);
            if (map_get(indegree, v) == NULL) {
                map_set(indegree, v, 0);
            }
            long in = (long) map_get(indegree, v);
            map_set(indegree, v, (void*) in+1);
        }
    }

    /* Putting nodes with zero in-degree in the queue */
    nodes_itr = list_itr_init(nodes);
    while (list_itr_has_next(nodes_itr)) {
        char *cur_node = list_itr_next(nodes_itr);
        if (map_get(indegree, cur_node) == 0) {
            list_push_back(queue, cur_node);
        }
    }

    /* Generating the topological ordering of the graph */
    while (!list_is_empty(queue)) {
        char *node = queue->head->val;
        list_pop_front(queue);
        list_push_back(topo_order, node);
        
        list_t *children = graph_get_children(graph, node);
        if (children == NULL) continue;

        /* For each child, increment its in degree by one */
        list_itr_t *children_itr = list_itr_init(children);
        while (list_itr_has_next(children_itr)) {
            char *child = (char*) list_itr_next(children_itr);
            long in = (long) map_get(indegree, child);
            map_set(indegree, child, (void*) in-1);
            if ((long) map_get(indegree, child) == 0) {
                list_push_back(queue, child);
            }
        }
    }

    return topo_order;
}
