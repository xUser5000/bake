#include<stdlib.h>
#include<stdio.h>

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
  if (rule == NULL) return;
  free(rule->target);

  list_itr_t *it = list_itr_init(rule->prerequisites);
  while (list_itr_has_next(it)) {
    free(list_itr_next(it));
  }
  list_itr_free(it);

  it = list_itr_init(rule->commands);
  while (list_itr_has_next(it)) {
    free(list_itr_next(it));
  }
  list_itr_free(it);

  list_free(rule->prerequisites);
  list_free(rule->commands);
  free(rule);
}
