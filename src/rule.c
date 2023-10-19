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

int rule_execute(rule_t *rule) {
  list_itr_t *cmd_itr = list_itr_init(rule->commands);
  while (list_itr_has_next(cmd_itr)) {
    char *cmd = list_itr_next(cmd_itr);
    printf("%s\n", cmd);
    int rc = system(cmd);
    if (rc != 0) {
      return 0;
    }
  }
  list_itr_free(cmd_itr);
  return 1;
}

void rule_free(rule_t *rule) {
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
