#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#include "e4c_lite.h"
#include "parser.h"


E4C_DEFINE_EXCEPTION(ParserException, "Parser Exception!", RuntimeException);
E4C_DEFINE_EXCEPTION(UnexpectedTokenException, "Unexpected Token!", ParserException);
E4C_DEFINE_EXCEPTION(NoRulesProvidedException, "No Rules Provided!", ParserException);
E4C_DEFINE_EXCEPTION(EmptyRuleException, "Empty Rule!", ParserException);

token_t* consume(parser_t *parser, token_type_t token_type);

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

token_t* consume(parser_t *parser, token_type_t token_type) {
    if (parser->lookahead->type != token_type) {
        E4C_THROW(UnexpectedTokenException, NULL);
    }
    token_t *res = parser->lookahead;
    parser->lookahead = tokenizer_read_token(parser->tokenizer);
    return res;
}

list_t* script(parser_t *parser) {
    optional_line_breaks(parser);
    return rule_list(parser);
}

list_t* rule_list(parser_t *parser) {
    list_t *rules = list_init();
    while (parser->lookahead->type != NULL_TOKEN && parser->lookahead->type != INVALID_TOKEN) {
        list_push(rules, (void*) rule(parser));
        optional_line_breaks(parser);
    }
    if (list_size(rules) == 0) {
        E4C_THROW(NoRulesProvidedException, NULL);
    }
    return rules;
}

rule_t* rule(parser_t *parser) {
    rule_t *r = rule_init();
    r->target = target(parser);
    optional_white_space(parser);
    colon(parser);
    optional_white_space(parser);
    r->prerequisites = prerequisite_list(parser);
    endline(parser);
    r->commands = command_list(parser);
    if (list_size(r->prerequisites) == 0 && list_size(r->commands) == 0) {
        E4C_THROW(EmptyRuleException, NULL);
    }
    return r;
}

char* target(parser_t *parser) { return consume(parser, IDENTIFIER)->content; }

char* colon(parser_t *parser) { return consume(parser, COLON)->content; }

list_t* prerequisite_list(parser_t *parser) {
    list_t *list = list_init();
    while (parser->lookahead->type == IDENTIFIER) {
        list_push(list, (void*) prerequisite(parser));
        optional_white_space(parser);
    }
    return list;
}

char* prerequisite(parser_t *parser) { return consume(parser, IDENTIFIER)->content; }

char* endline(parser_t *parser) { return consume(parser, ENDLINE)->content; }

char* tab(parser_t *parser) { return consume(parser, TAB)->content; }

list_t* command_list(parser_t *parser) {
    list_t *list = list_init();
    while (parser->lookahead->type == TAB) {
        tab(parser);
        list_push(list, (void*) command(parser));
        optional_white_space(parser);
        endline(parser);
    }
    if (parser->lookahead->type == WHITE_SPACE) {
        E4C_THROW(UnexpectedTokenException, "Unexpected whitespace");
    }
    return list;
}

char* command(parser_t *parser) {
    char *command_with_quotes = consume(parser, COMMAND)->content;
    int sz = strlen(command_with_quotes);
    char *command_without_quotes = command_with_quotes + 1;
    command_with_quotes[sz - 1] = '\0';
    return command_without_quotes;
}

int optional_white_space(parser_t *parser) {
    if (parser->lookahead->type == WHITE_SPACE) {
        consume(parser, WHITE_SPACE);
        return 1;
    }
    return 0;
}

int optional_line_breaks(parser_t *parser) {
    int res = 0;
    while (parser->lookahead->type == ENDLINE) {
        res = 1;
        consume(parser, ENDLINE);
    }
    return res;
}
