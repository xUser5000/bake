#include "common.h"
#include "utest.h"
#include "graph.h"
#include "list.h"

UTEST_MAIN();

UTEST(Graph, AddEdge) {
    graph_t *g = graph_init();
    graph_add_edge(g, "a", "b");
    graph_add_edge(g, "a", "c");

    char* children[] = {"b", "c"};
    list_t *children_list = graph_get_children(g, "a");
    ASSERT_LIST_EQ(children_list, children);

    graph_free(g);
}

UTEST(Graph, HasCycle) {
  graph_t *g = graph_init();
  graph_add_edge(g, "a", "b");
  graph_add_edge(g, "b", "c");
  graph_add_edge(g, "c", "a");
  ASSERT_TRUE(graph_has_cycle(g));
  graph_free(g);
  g = NULL;

  g = graph_init();
  graph_add_edge(g, "a", "b");
  graph_add_edge(g, "b", "c");
  graph_add_edge(g, "c", "d");
  ASSERT_FALSE(graph_has_cycle(g));

  graph_free(g);
}

UTEST(Graph, TopoOrder) {
    graph_t *g = graph_init();
    graph_add_edge(g, "a", "b");
    graph_add_edge(g, "a", "c");
    graph_add_edge(g, "b", "d");
    graph_add_edge(g, "c", "d");
    graph_add_edge(g, "d", "e");
    graph_add_edge(g, "d", "f");
    graph_add_edge(g, "e", "g");
    graph_add_edge(g, "f", "g");

    char* expected_order[] = {"a", "b", "c", "d", "e", "f", "g"};
    list_t *order = graph_topo_order(g, "g");

    ASSERT_EQ(list_size(order), 7);
    ASSERT_LIST_EQ(order, expected_order);

    list_free(order);
    graph_free(g);
}
