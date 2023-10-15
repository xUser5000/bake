#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#include "tokenizer.h"
#include "parser.h"
#include "graph.h"

int main(int argc, char* argv[]) {
    char *bakefile_path = NULL;

    if (argc == 1) {
        bakefile_path = strcpy(bakefile_path, "Bakefile");
    }

    if (argc == 2) {
        bakefile_path = argv[1];
    }

    if (argc > 2) {
        printf("bake: too many arguments!\n");
        exit(1);
    }

    FILE *makefile = fopen(bakefile_path, "r");
    if (makefile == NULL) {
        printf("bake: no such file: %s", bakefile_path);
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

    // execute targets in topological order
    list_t *order = graph_topo_order(graph);
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
