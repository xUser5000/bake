#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#include "tokenizer.h"
#include "parser.h"
#include "graph.h"

int main(int argc, char* argv[]) {
    if (argc > 2) {
        printf("bake: too many arguments!\n");
        exit(1);
    }

    const char *bakefile_path = "Bakefile";
    FILE *makefile = fopen(bakefile_path, "r");
    if (makefile == NULL) {
        printf("bake: Baekfile not found\n");
        exit(1);
    }

    tokenizer_t *tokenizer = tokenizer_init(makefile);
    parser_t *parser = parser_init(tokenizer);
    list_t *rules = parser_get_rules(parser);

    map_t *target_to_rule = map_init();
    graph_t *graph = graph_init();

    // build dependency graph
    list_itr_t *rules_itr = list_itr_init(rules);
    while (list_itr_has_next(rules_itr)) {
        rule_t *rule = list_itr_next(rules_itr);

        map_set(target_to_rule, rule->target, rule);
        graph_add_node(graph, rule->target);
        
        list_itr_t *pre_itr = list_itr_init(rule->prerequisites);
        while (list_itr_has_next(pre_itr)) {
            char *pre = list_itr_next(pre_itr);
            graph_add_edge(graph, pre, rule->target);
        }
    }

    // if graph has a cycle, report and exit
    if (graph_has_cycle(graph)) {
      printf("bake: dependency graph has a cycle!\n");
      exit(1);
    }

    // execute target and its prerequisites in topological order
    char *root_target = (argc == 2) ? argv[1] : ((rule_t*) rules->head->val)->target;
    list_t *order = graph_topo_order(graph, root_target);
    list_itr_t *order_itr = list_itr_init(order);
    while (list_itr_has_next(order_itr)) {
        char *target = list_itr_next(order_itr);
        
        rule_t *rule = map_get(target_to_rule, target);
        list_itr_t *cmd_itr = list_itr_init(rule->commands);
        while (list_itr_has_next(cmd_itr)) {
            char *cmd = list_itr_next(cmd_itr);
            printf("%s\n", cmd);
            int rc = system(cmd);
            if (rc != 0) {
                break;
            }
        }
    }

    return 0;
}
