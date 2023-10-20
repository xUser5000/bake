#pragma once

#include<stdlib.h>
#include<stdio.h>

#include "list.h"
#include "rule.h"


int run(rule_t *rule) {
  list_itr_t *cmd_itr = list_itr_init(rule->commands);
  while (list_itr_has_next(cmd_itr)) {
    char *cmd = (char*) list_itr_next(cmd_itr);
    printf("%s\n", cmd);
    int rc = system(cmd);
    if (rc != 0) {
      return 0;
    }
  }
  list_itr_free(cmd_itr);
  return 1;
}
