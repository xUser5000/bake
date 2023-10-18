#pragma once

#include "list.h"

typedef struct rule_t {
    char *target;
    list_t *prerequisites;
    list_t *commands;
} rule_t;

rule_t* rule_init(void);

void rule_free(rule_t *rule);
