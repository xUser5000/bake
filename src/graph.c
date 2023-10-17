#include<stdio.h>
#include<stdlib.h>

#include "graph.h"
#include "list.h"
#include "map.h"

void graph_reverse(graph_t *graph, graph_t *reversed_graph);
void graph_dfs(graph_t *graph, char *node, map_t *visisted, list_t *list);
int graph_has_cycle_internal(graph_t *graph, char *node, map_t *visited, map_t *instack);

graph_t* graph_init(void) {
    graph_t *graph = malloc(sizeof(graph_t));
    graph->adj = map_init();
    return graph;
}

void graph_add_node(graph_t *graph, char *node) {
    if (map_get(graph->adj, node) == NULL) {
        map_set(graph->adj, node, list_init());
    }
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

int graph_has_cycle(graph_t *graph) {
    map_t *visited = map_init();
    map_t *instack = map_init();
    char *random_node = map_keys(graph->adj)->head->val;
    return graph_has_cycle_internal(graph, random_node, visited, instack);
}

int graph_has_cycle_internal(graph_t *graph, char *node, map_t *visited, map_t *instack) {
  map_set(visited, node, (void*) 1);
  map_set(instack, node, (void*) 1);

  int ans = 0;
  list_t *children = graph_get_children(graph, node);
  if (children == NULL) return 0;
  list_itr_t *children_itr = list_itr_init(children);
  while (list_itr_has_next(children_itr)) {
    char *child = (char*) list_itr_next(children_itr);
    if (map_get(instack, child)) {
      return 1;
    }
    if (map_get(visited, child)) {
      continue;
    }
    ans = ans || graph_has_cycle_internal(graph, child, visited, instack);
  }

  map_set(instack, node, 0);
  return ans;
}

list_t* graph_topo_order(graph_t *graph, char *root) {
    list_t *topo_order = list_init();

    /* Reverse the subgraph rooted at (char *root) */
    graph_t *reversed_graph = graph_init();
    graph_reverse(graph, reversed_graph);

    /* Perform Topological ordering */
    map_t *visited = map_init();
    graph_dfs(reversed_graph, root, visited, topo_order);
    map_free(visited);

    return topo_order;
}

void graph_dfs(graph_t *graph, char *node, map_t *visited, list_t *list) {
    map_set(visited, node, (void*) 1);

    list_t *children = graph_get_children(graph, node);
    list_itr_t *children_itr = list_itr_init(children);
    while (list_itr_has_next(children_itr)) {
        char *child = (char*) list_itr_next(children_itr);
        if (map_get(visited, child) == NULL) {
            graph_dfs(graph, child, visited, list);
        }
    }

    list_push_back(list, node);
}

void graph_reverse(graph_t *graph, graph_t *reversed_graph) {
    list_t *nodes = map_keys(graph->adj);
    list_itr_t *nodes_itr = list_itr_init(nodes);
    while (list_itr_has_next(nodes_itr)) {
        char *node = (char*) list_itr_next(nodes_itr);
        graph_add_node(reversed_graph, node);

        list_t *children = graph_get_children(graph, node);
        list_itr_t *children_itr = list_itr_init(children);
        while (list_itr_has_next(children_itr)) {
            char *child = (char*) list_itr_next(children_itr);
            graph_add_node(reversed_graph, child);
            graph_add_edge(reversed_graph, child, node);
        }

        list_itr_free(children_itr);
        list_free(children, 0);
    }

    list_itr_free(nodes_itr);
    list_free(nodes, 0);
}

void graph_free(graph_t *graph) {
    list_t *nodes = map_keys(graph->adj);
    list_itr_t *it = list_itr_init(nodes);
    while (list_itr_has_next(it)) {
        char *node = (char*) list_itr_next(it);
        list_free(map_get(graph->adj, node), 0);
        map_set(graph->adj, node, NULL);
    }
    list_itr_free(it);
    list_free(nodes, 0);
    map_free(graph->adj);
    free(graph);
}

void graph_print(graph_t *graph) {
    list_t *nodes = map_keys(graph->adj);
    list_itr_t *nodes_itr = list_itr_init(nodes);
    while (list_itr_has_next(nodes_itr)) {
        char *node = list_itr_next(nodes_itr);
        printf("%s: ", node);
        list_itr_t *children_itr = list_itr_init(graph_get_children(graph, node));
        while (list_itr_has_next(children_itr)) {
            char *child = list_itr_next(children_itr);
            printf("%s ", child);
        }
        printf("\n");
        list_itr_free(children_itr);
    }
    list_itr_free(nodes_itr);
    list_free(nodes, 0);
}
