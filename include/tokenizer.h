#pragma once

#include<stdio.h>

typedef struct tokenizer_t {
    FILE *in;
    char *cur_line;
    size_t line_size;
    int line_number;
    int index;
    int done;
} tokenizer_t;

typedef enum token_type_t {
    NULL_TOKEN, INVALID_TOKEN, IDENTIFIER, COLON, ENDLINE, TAB, COMMAND, WHITE_SPACE
} token_type_t;

#define TOKENS_TYPES_CNT 8

typedef struct token_t {
    char *content;
    enum token_type_t type;
} token_t;

token_t* token_init(char *content, token_type_t type);

tokenizer_t* tokenizer_init(FILE *in);

token_t* tokenizer_read_token(tokenizer_t *tokenizer);

int tokenizer_get_line_number(tokenizer_t *tokenizer);

int tokenizer_get_index(tokenizer_t *tokenizer);
