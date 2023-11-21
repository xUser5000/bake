#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#include "e4c_lite.h"
#include "list.h"
#include "map.h"
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

    // parse the bakefile and extract rules
    list_t *rules = NULL;
    E4C_TRY {
        rules = parser_get_rules(parser);
    } E4C_CATCH(ParserException) {
        printf("%s\n", E4C_EXCEPTION.message);
        exit(1);
    }

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
    list_itr_free(rules_itr);
    rules_itr = NULL;

    // if graph has a cycle, report and exit
    if (graph_has_cycle(graph)) {
      printf("bake: dependency graph has a cycle!\n");
      exit(1);
    }

    // execute target and its prerequisites in topological order
    char *root_target = (argc == 2) ? argv[1] : ((rule_t*) rules->head->val)->target;
    if (map_get(target_to_rule, root_target) == NULL) {
      printf("bake: rule \"%s\" is not defined\n", root_target);
      exit(1);
    }

    graph_run(graph, target_to_rule, root_target);

    rules_itr = list_itr_init(rules);
    while (list_itr_has_next(rules_itr)) {
        rule_t *rule = list_itr_next(rules_itr);
        rule_free(rule);
    }
    list_itr_free(rules_itr);
    
    list_free(rules);
    map_free(target_to_rule);
    graph_free(graph);

    return 0;
}
