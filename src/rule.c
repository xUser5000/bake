#include<stdlib.h>

#include "rule.h"
#include "list.h"

rule_t* rule_init(void) {
    rule_t *rule = (rule_t*) malloc(sizeof(rule_t));
    rule->target = NULL;
    rule->prerequisites = NULL;
    rule->commands = NULL;
    return rule;
}

void rule_free(rule_t *rule) {
  list_free(rule->prerequisites);
  list_free(rule->commands);
  free(rule);
}
