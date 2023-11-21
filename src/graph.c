#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<time.h>
#include<unistd.h>
#include<pthread.h>

#include "rule.h"
#include "graph.h"
#include "list.h"
#include "map.h"
#include "run.h"

typedef struct run_data_t {
  graph_t *graph;
  map_t *target_to_rule;
  char *node;
  map_t *visited;
  int exec_status;
} run_data_t;

graph_t* graph_reverse(graph_t *graph);

void graph_dfs(graph_t *graph, char *node, map_t *visisted, list_t *list);

int graph_has_cycle_internal(graph_t *graph, char *node, map_t *visited, map_t *instack);

int graph_run_internal(graph_t *graph, map_t *target_to_rule, char *node, map_t *visited);

void *graph_run_internal_wrapper(void *arg);

int is_file(char *pathname);

int mod_time_less(char *file1, char *file2);


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

    list_t *nodes = map_keys(graph->adj);
    char *random_node = nodes->head->val;
    list_free(nodes);

    int ret = graph_has_cycle_internal(graph, random_node, visited, instack);
    map_free(visited);
    map_free(instack);

    return ret;
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
      ans = 1;
      break;
    }
    if (map_get(visited, child)) {
      continue;
    }
    ans = ans || graph_has_cycle_internal(graph, child, visited, instack);
  }
  list_itr_free(children_itr);

  map_set(instack, node, 0);

  return ans;
}

list_t* graph_topo_order(graph_t *graph, char *root) {
    list_t *topo_order = list_init();

    /* Reverse the subgraph rooted at (char *root) */
    graph_t *reversed_graph = graph_reverse(graph);

    /* Perform Topological ordering */
    map_t *visited = map_init();
    graph_dfs(reversed_graph, root, visited, topo_order);
    graph_free(reversed_graph);
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
    list_itr_free(children_itr);

    list_push_back(list, node);
}

graph_t* graph_reverse(graph_t *graph) {
    graph_t *reversed_graph = graph_init();
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
        children = NULL;
    }

    list_itr_free(nodes_itr);
    list_free(nodes);
    return reversed_graph;
}

void graph_run(graph_t *graph, map_t *target_to_rule, char *root_target) {
  graph_t *reversed_graph = graph_reverse(graph);
  map_t *visited = map_init();
  graph_run_internal(reversed_graph, target_to_rule, root_target, visited);
  map_free(visited);
  graph_free(reversed_graph);
}

int graph_run_internal(graph_t *graph, map_t *target_to_rule, char *node, map_t *visited) {
  map_set(visited, node, (void*) 1);

  list_t *children = graph_get_children(graph, node);
  list_t *threads = list_init();
  list_t *objects = list_init();

  list_itr_t *children_itr = list_itr_init(children);
  while (list_itr_has_next(children_itr)) {
    char *child = (char*) list_itr_next(children_itr);
    if (map_get(visited, child) == NULL) {
      run_data_t *obj = (run_data_t*) malloc(sizeof(run_data_t));
      obj->graph = graph;
      obj->target_to_rule = target_to_rule;
      obj->node = child;
      obj->visited = visited;

      pthread_t *th = (pthread_t*) malloc(sizeof(pthread_t));
      pthread_create(th, NULL, graph_run_internal_wrapper, obj);
//      pthread_join(th, NULL);

      list_push_back(threads, th);
      list_push_back(objects, &obj);
    }
  }
  list_itr_free(children_itr);

  // wait for all threads to finish
  list_itr_t *threads_itr = list_itr_init(threads);
  while (list_itr_has_next(threads_itr)) {
    pthread_t *th = (pthread_t*) list_itr_next(threads_itr);
    pthread_join(*th, NULL);
  }
  list_itr_free(threads_itr);

  // if at least one child failed, abort the whole process
  list_itr_t *objects_itr = list_itr_init(objects);
  while (list_itr_has_next(objects_itr)) {
    run_data_t *obj = list_itr_next(objects_itr);
    if (obj->exec_status == 0) return 0;
  }
  list_itr_free(objects_itr);

  // determine if target is out of date
  int out_of_date = !is_file(node);
  children_itr = list_itr_init(children);
  while (list_itr_has_next(children_itr)) {
    char *child = (char*) list_itr_next(children_itr);
    if (map_get(visited, child) == NULL) {
      if (
          !is_file(node) ||
          !is_file(child) ||
          mod_time_less(node, child)
          ) {
        out_of_date = 1;
      }
    }
  }
  list_itr_free(children_itr);

  // if out-of-date, build target
  if (out_of_date) {
    rule_t *node_rule = map_get(target_to_rule, node);
    if (node_rule == NULL) {
      printf("bake: target %s is not defined\n", node);
      exit(1);
    }
    return run(node_rule);
  }

  return 1;
}


void *graph_run_internal_wrapper(void *arg) {
  run_data_t *data = (run_data_t *) arg;
  int exec_status = graph_run_internal(
      data->graph,
      data->target_to_rule,
      data->node,
      data->visited
      );
  data->exec_status = exec_status;
  return NULL;
}

int is_file(char *pathname) {
  struct stat sb;
  return stat(pathname, &sb) == 0 && S_ISREG(sb.st_mode);
}

int mod_time_less(char *file1, char *file2) {
  struct stat sb1;
  struct stat sb2;
  if (stat(file1, &sb1) == 1) return 0;
  if (stat(file2, &sb2) == 1) return 0;
  struct timespec t1 = sb1.st_mtim;
  struct timespec t2 = sb2.st_mtim;

  if (t1.tv_sec != t2.tv_sec) return t1.tv_sec < t2.tv_sec;
  if (t1.tv_nsec != t2.tv_nsec) return t1.tv_nsec < t2.tv_nsec;
  return 0;
}

void graph_free(graph_t *graph) {
    if (graph == NULL) return;
    list_t *nodes = map_keys(graph->adj);
    list_itr_t *it = list_itr_init(nodes);
    while (list_itr_has_next(it)) {
        char *node = (char*) list_itr_next(it);
        list_free(map_get(graph->adj, node));
        map_set(graph->adj, node, NULL);
    }
    list_itr_free(it);
    list_free(nodes);
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
    list_free(nodes);
}
