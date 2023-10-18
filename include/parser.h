#pragma once

#include "e4c_lite.h"
#include "tokenizer.h"
#include "rule.h"

E4C_DECLARE_EXCEPTION(ParserException);
E4C_DECLARE_EXCEPTION(UnexpectedTokenException);
E4C_DECLARE_EXCEPTION(NoRulesProvidedException);
E4C_DECLARE_EXCEPTION(EmptyRuleException);

typedef struct parser_t {
    token_t *lookahead;
    tokenizer_t *tokenizer;
} parser_t;

parser_t* parser_init(tokenizer_t *tokenizer);

list_t* parser_get_rules(parser_t *parser);

void parser_free(parser_t *parser);
