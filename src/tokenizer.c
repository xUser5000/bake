#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<pcre.h>

#include "tokenizer.h"

const char *TOKEN_PATTERNS[] = {NULL, NULL, "^[\\w-_.]+", "^:", "^\\n", "^\\t", "^\".+\"", "^[ ]+"};

int is_blank(char *str);

token_t* token_init(char *content, token_type_t type) {
    token_t *token = (token_t*) malloc(sizeof(token_t));
    token->content = content;
    token->type = type;
    return token;
}

void token_free(token_t *token) {
  free(token->content);
  free(token);
}

tokenizer_t* tokenizer_init(FILE *in) {
    tokenizer_t *tokenizer = (tokenizer_t*) malloc(sizeof(tokenizer_t));
    tokenizer->in = in;
    tokenizer->cur_line = NULL;
    tokenizer->line_size = getline(&tokenizer->cur_line, &tokenizer->line_size, tokenizer->in);
    tokenizer->line_number = 0;
    tokenizer->index = 0;
    tokenizer->done = 0;
    return tokenizer;
}

token_t* tokenizer_read_token(tokenizer_t *tokenizer) {
    if (tokenizer->done) return token_init(NULL, NULL_TOKEN);
    if (tokenizer->index == tokenizer->line_size) {
        size_t rv = getline(&tokenizer->cur_line, &tokenizer->line_size, tokenizer->in);
        if (rv == (size_t) -1) {
            tokenizer->done = 1;
            return token_init(strdup("\n"), ENDLINE);
        }
        tokenizer->line_size = rv;
        tokenizer->line_number++;

        if (is_blank(tokenizer->cur_line)) {
            tokenizer->index = tokenizer->line_size;
            return tokenizer_read_token(tokenizer);
        }

        tokenizer->index = 0;
    }

    for (int i = 2; i < TOKENS_TYPES_CNT; i++) {
        const char* error;
        int error_offset;

        pcre* re = pcre_compile(TOKEN_PATTERNS[i], 0, &error, &error_offset, NULL);
        if (re == NULL) {
            printf("PCRE compilation failed at offset %d: %s\n", error_offset, error);
            exit(1);
        }

        pcre_extra* extra = NULL;
        char *subject = tokenizer->cur_line + tokenizer->index;
        int subject_length = strlen(subject);
        int start_offset = 0;
        int ovector[30];  // Space for capturing subpatterns
        int rc = pcre_exec(re, extra, subject, subject_length, start_offset, 0, ovector, 30);

        pcre_free(re);
        re = NULL;

        if (rc == PCRE_ERROR_NOMATCH) continue;
        if (rc < 0) {
            printf("PCRE matching error: %d\n", rc);
            exit(1);
        }

        int match_length = ovector[1] - ovector[0];
        tokenizer->index += match_length;
        char *content = strndup(subject, match_length);
        token_t *ret = token_init(content, (token_type_t) i);
        return ret;
    }

    return token_init(strdup(tokenizer->cur_line + tokenizer->index), INVALID_TOKEN);
}

int tokenizer_get_line_number(tokenizer_t *tokenizer) { return tokenizer->line_number; }

int tokenizer_get_index(tokenizer_t *tokenizer) { return tokenizer->index; }

void tokenizer_free(tokenizer_t *tokenizer) {
  fclose(tokenizer->in);
  free(tokenizer->cur_line);
  free(tokenizer);
}

int is_blank(char *str) {
    for (char *c = str; *c; c++) {
        if (*c != '\n' && *c != ' ' && *c != '\t') {
            return 0;
        }
    }
    return 1;
}
