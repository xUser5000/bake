#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#include "common.h"
#include "e4c_lite.h"
#include "utest.h"
#include "rule.h"
#include "list.h"
#include "tokenizer.h"
#include "parser.h"

UTEST_MAIN();

UTEST(Parser, BasicScriptWithoutWhiteSpaces) {
    char* test_string = "target:dependency\n\t\"recipe\"";
    FILE *ss = fmemopen(test_string, strlen(test_string), "r");
    tokenizer_t *tokenizer = tokenizer_init(ss);
    parser_t *parser = parser_init(tokenizer);

    list_t *rules = parser_get_rules(parser);
    rule_t *rule = (rule_t*) rules->head->val;
    ASSERT_STREQ(rule->target, "target");
    ASSERT_STREQ((char*) rule->prerequisites->head->val, "dependency");
    ASSERT_STREQ((char*) rule->commands->head->val, "recipe");

    rule_free(rule);
    rules->head->val = NULL;

    list_free(rules);
    rules = NULL;

    parser_free(parser);
}

UTEST(Parser, BasicScriptWithWhiteSpaces) {
    char* test_string = "target  :dependency     \n\t\"recipe\"";
    FILE *ss = fmemopen(test_string, strlen(test_string), "r");
    tokenizer_t *tokenizer = tokenizer_init(ss);
    parser_t *parser = parser_init(tokenizer);

    list_t *rules = parser_get_rules(parser);
    rule_t *rule = (rule_t*) rules->head->val;
    ASSERT_STREQ(rule->target, "target");
    ASSERT_STREQ((char*) rule->prerequisites->head->val, "dependency");
    ASSERT_STREQ((char*) rule->commands->head->val, "recipe");

    rule_free(rule);
    list_free(rules);
    parser_free(parser);
}


UTEST(Parser, MultipleDependenciesAndMultipleRecipes) {
    char* test_string = "target  :dependency1 dependency2    \n\t\"recipe1\" \n\t\"recipe2\"";
    FILE *ss = fmemopen(test_string, strlen(test_string), "r");
    tokenizer_t *tokenizer = tokenizer_init(ss);
    parser_t *parser = parser_init(tokenizer);

    list_t *rules = parser_get_rules(parser);
    rule_t *rule = (rule_t*) rules->head->val;
    ASSERT_STREQ(rule->target, "target");

    char* arr1[] = {"dependency1", "dependency2"};
    ASSERT_LIST_EQ(rule->prerequisites, arr1);

    char* arr2[] = {"recipe1", "recipe2"};
    ASSERT_LIST_EQ(rule->commands, arr2);

    rule_free(rule);
    list_free(rules);
    parser_free(parser);
}


UTEST(Parser, InvalidScript) {
    char* test_string = "target  :dependency1 dependency2    \n \t\"recipe1\" \n\t\"recipe2\"";
    FILE *ss = fmemopen(test_string, strlen(test_string), "r");
    tokenizer_t *tokenizer = tokenizer_init(ss);
    parser_t *parser = parser_init(tokenizer);
    
    list_t *rules;
    ASSERT_THROWS((rules = parser_get_rules(parser)), UnexpectedTokenException);

}


UTEST(Parser, MultipleRules) {
    char* test_string = "target1:dependency1\n\t\"recipe1\"\ntarget2:dependency2\n\t\"recipe2\"\n\t\"recipe3\"";
    FILE *ss = fmemopen(test_string, strlen(test_string), "r");
    tokenizer_t *tokenizer = tokenizer_init(ss);
    parser_t *parser = parser_init(tokenizer);
    
    list_t *rules = parser_get_rules(parser);
    rule_t *rule1 = (rule_t*) rules->head->val;
    rule_t *rule2 = (rule_t*) rules->head->next->val;

    ASSERT_EQ(list_size(rules), 2);

    ASSERT_STREQ(rule1->target, "target1");
    ASSERT_LIST_EQ(rule1->prerequisites, (char*[]) {"dependency1"});
    ASSERT_LIST_EQ(rule1->commands, (char*[]) {"recipe1"});
    
    ASSERT_STREQ(rule2->target, "target2");
    ASSERT_LIST_EQ(rule2->prerequisites, (char*[]) {"dependency2"});
    char* commands[] = {"recipe2", "recipe3"};
    ASSERT_LIST_EQ(rule2->commands, commands);

    rule_free(rule1);
    rule_free(rule2);
    list_free(rules);
    parser_free(parser);
}

UTEST(Parser, MultipleRulesWithEmptyLinesInBetween) {
    char* test_string = "\n\ntarget1:dependency1\n\t\"recipe1\"\n\n\ntarget2:dependency2\n\t\"recipe2\"\n\t\"recipe3\"\n\n";
    FILE *ss = fmemopen(test_string, strlen(test_string), "r");
    tokenizer_t *tokenizer = tokenizer_init(ss);
    parser_t *parser = parser_init(tokenizer);
    
    list_t *rules = parser_get_rules(parser);
    rule_t *rule1 = (rule_t*) rules->head->val;
    rule_t *rule2 = (rule_t*) rules->head->next->val;

    ASSERT_EQ(list_size(rules), 2);

    ASSERT_STREQ(rule1->target, "target1");
    ASSERT_LIST_EQ(rule1->prerequisites, (char*[]) {"dependency1"});
    ASSERT_LIST_EQ(rule1->commands, (char*[]) {"recipe1"});
    
    ASSERT_STREQ(rule2->target, "target2");
    ASSERT_LIST_EQ(rule2->prerequisites, (char*[]) {"dependency2"});
    char* commands[] = {"recipe2", "recipe3"};
    ASSERT_LIST_EQ(rule2->commands, commands);

    rule_free(rule1);
    rule_free(rule2);
    list_free(rules);
    parser_free(parser);
}


UTEST(Parser, NoRules) {
    char* test_string = "";
    FILE *ss = fmemopen(test_string, strlen(test_string), "r");
    tokenizer_t *tokenizer = tokenizer_init(ss);
    parser_t *parser = parser_init(tokenizer);

    ASSERT_THROWS(parser_get_rules(parser), NoRulesProvidedException);

}


UTEST(Parser, NoPrerequisites) {
    char* test_string = "target : \n\t\"recipe1\"\n\t\"recipe2\"";
    FILE *ss = fmemopen(test_string, strlen(test_string), "r");
    tokenizer_t *tokenizer = tokenizer_init(ss);
    parser_t *parser = parser_init(tokenizer);
    
    list_t *rules = parser_get_rules(parser);
    ASSERT_EQ(list_size(rules), 1);
    rule_t *rule = (rule_t*) rules->head->val;

    ASSERT_STREQ(rule->target, "target");
    ASSERT_LIST_EQ(rule->prerequisites, (char*[]) {});
    char* commands[] = {"recipe1", "recipe2"};
    ASSERT_LIST_EQ(rule->commands, commands);

    rule_free(rule);
    list_free(rules);
    parser_free(parser);
}


UTEST(Parser, NoCommands) {
    char* test_string = "target : dependency1 dependency2 \n";
    FILE *ss = fmemopen(test_string, strlen(test_string), "r");
    tokenizer_t *tokenizer = tokenizer_init(ss);
    parser_t *parser = parser_init(tokenizer);
    
    list_t *rules = parser_get_rules(parser);
    ASSERT_EQ(list_size(rules), 1);
    rule_t *rule = (rule_t*) rules->head->val;

    ASSERT_STREQ(rule->target, "target");
    char* dependencies[] = {"dependency1", "dependency2"};
    ASSERT_LIST_EQ(rule->prerequisites, dependencies);
    ASSERT_LIST_EQ(rule->commands, (char* []) {});

    rule_free(rule);
    list_free(rules);
    parser_free(parser);
}


UTEST(Parser, NoDependenciesNoRules) {
    char* test_string = "target : \n";
    FILE *ss = fmemopen(test_string, strlen(test_string), "r");
    tokenizer_t *tokenizer = tokenizer_init(ss);
    parser_t *parser = parser_init(tokenizer);
    ASSERT_THROWS(parser_get_rules(parser), EmptyRuleException);

}

UTEST(Parser, IgnoreEmptyLines) {
  char *test_string = "A:B\n\t\n   \n";
  FILE *ss = fmemopen(test_string, strlen(test_string), "r");
  tokenizer_t *tokenizer = tokenizer_init(ss);
  parser_t *parser = parser_init(tokenizer);

  list_t *rules = parser_get_rules(parser);
  rule_t *r = (rule_t *) rules->head->val;
  char* pre = (char*) r->prerequisites->head->val;

  ASSERT_STREQ(r->target, "A");
  ASSERT_STREQ(pre, "B");

  rule_free(r);
  list_free(rules);
  parser_free(parser);
}
