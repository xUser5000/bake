#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#include "e4c_lite.h"
#include "parser.h"
#include "rule.h"
#include "tokenizer.h"


E4C_DEFINE_EXCEPTION(ParserException, "Parser Exception!", RuntimeException);
E4C_DEFINE_EXCEPTION(UnexpectedTokenException, "Unexpected Token!", ParserException);
E4C_DEFINE_EXCEPTION(NoRulesProvidedException, "No Rules Provided!", ParserException);
E4C_DEFINE_EXCEPTION(EmptyRuleException, "Empty Rule!", ParserException);

token_t* consume(parser_t *parser, token_type_t token_type);

char* get_content_and_free(token_t *t);

list_t* script(parser_t *parser);
list_t* rule_list(parser_t *parser);
rule_t* rule(parser_t *parser);
char* target(parser_t *parser);
char* colon(parser_t *parser);
list_t* prerequisite_list(parser_t *parser);
char* prerequisite(parser_t *parser);
char* endline(parser_t *parser);
char* tab(parser_t *parser);
list_t* command_list(parser_t *parser);
char* command(parser_t *parser);
char* endline_or_eof(parser_t *parser);
int optional_white_space(parser_t *parser);
int optional_line_breaks(parser_t *parser);


parser_t* parser_init(tokenizer_t *tokenizer) {
    parser_t *parser = (parser_t*) malloc(sizeof(parser_t));
    parser->lookahead = NULL;
    parser->tokenizer = tokenizer;
    return parser;
}

list_t* parser_get_rules(parser_t *parser) {
    parser->lookahead = tokenizer_read_token(parser->tokenizer);
    return script(parser);
}

void parser_free(parser_t *parser) {
  token_free(parser->lookahead);
  tokenizer_free(parser->tokenizer);
  free(parser);
}

token_t* consume(parser_t *parser, token_type_t token_type) {
    if (parser->lookahead->type != token_type) {
        char buff[100];
        sprintf(buff, "bake: unexpected token on line %zu", parser->tokenizer->line_number + 1);
        parser_free(parser);
        E4C_THROW(UnexpectedTokenException, buff);
    }
    token_t *res = token_init(strdup(parser->lookahead->content), parser->lookahead->type);
    token_free(parser->lookahead);
    parser->lookahead = tokenizer_read_token(parser->tokenizer);
    return res;
}

char* get_content_and_free(token_t *t) {
    char *ret = strdup(t->content);
    token_free(t);
    return ret;
}

list_t* script(parser_t *parser) {
    optional_line_breaks(parser);
    return rule_list(parser);
}

list_t* rule_list(parser_t *parser) {
    list_t *rules = list_init();
    while (parser->lookahead->type != NULL_TOKEN && parser->lookahead->type != INVALID_TOKEN) {
        list_push_back(rules, (void*) rule(parser));
        optional_line_breaks(parser);
    }
    if (list_size(rules) == 0) {
        list_free(rules);
        parser_free(parser);
        E4C_THROW(NoRulesProvidedException, NULL);
    }
    return rules;
}

rule_t* rule(parser_t *parser) {
    rule_t *r = rule_init();
    r->target = target(parser);
    optional_white_space(parser);
    free(colon(parser));
    optional_white_space(parser);
    r->prerequisites = prerequisite_list(parser);
    free(endline(parser));
    r->commands = command_list(parser);
    if (list_size(r->prerequisites) == 0 && list_size(r->commands) == 0) {
        char buff[100];
        sprintf(buff, "bake: rule %s is empty!", r->target);
        rule_free(r);
        parser_free(parser);
        E4C_THROW(EmptyRuleException, buff);
    }
    return r;
}

char* target(parser_t *parser) { return get_content_and_free(consume(parser, IDENTIFIER)); }

char* colon(parser_t *parser) { return get_content_and_free(consume(parser, COLON)); }

list_t* prerequisite_list(parser_t *parser) {
    list_t *list = list_init();
    while (parser->lookahead->type == IDENTIFIER) {
        list_push_back(list, (void*) prerequisite(parser));
        optional_white_space(parser);
    }
    return list;
}

char* prerequisite(parser_t *parser) { return get_content_and_free(consume(parser, IDENTIFIER)); }

char* endline(parser_t *parser) { return get_content_and_free(consume(parser, ENDLINE)); }

char* tab(parser_t *parser) { return get_content_and_free(consume(parser, TAB)); }

list_t* command_list(parser_t *parser) {
    list_t *list = list_init();
    while (parser->lookahead->type == TAB) {
        free(tab(parser));
        list_push_back(list, (void*) command(parser));
        optional_white_space(parser);
        free(endline(parser));
    }
    if (parser->lookahead->type == WHITE_SPACE) {
        char buff[100];
        sprintf(buff, "bake: Unexpected whitespace on line %zu", parser->tokenizer->line_number + 1);
        list_free(list);
        parser_free(parser);
        E4C_THROW(UnexpectedTokenException, buff);
    }
    return list;
}

char* command(parser_t *parser) {
    char *command_with_quotes = get_content_and_free(consume(parser, COMMAND));
    int sz = strlen(command_with_quotes);
    char *command_without_quotes = strndup(command_with_quotes + 1, sz - 2);
    free(command_with_quotes);
    return command_without_quotes;
}

int optional_white_space(parser_t *parser) {
    if (parser->lookahead->type == WHITE_SPACE) {
        char *whitespace = get_content_and_free(consume(parser, WHITE_SPACE));
        free(whitespace);
        return 1;
    }
    return 0;
}

int optional_line_breaks(parser_t *parser) {
    int res = 0;
    while (parser->lookahead->type == ENDLINE) {
        res = 1;
        char *endl = get_content_and_free(consume(parser, ENDLINE));
        free(endl);
    }
    return res;
}
