#include<stdlib.h>

#include "rule.h"

rule_t* rule_init(void) {
    rule_t *rule = (rule_t*) malloc(sizeof(rule_t));
    rule->target = NULL;
    rule->prerequisites = NULL;
    rule->commands = NULL;
    return rule;
}
