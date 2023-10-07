#include<string.h>
#include<pcre.h>

#include "tokenizer.h"


const char *TOKEN_PATTERNS[] = {NULL, NULL, "^[\\w-_.]+", "^:", "^\\n", "^\\t", "^\".+\"", "^[ ]+"};

token_t* token_init(char *content, token_type_t type) {
    token_t *token = (token_t*) malloc(sizeof(token_t));
    token->content = content;
    token->type = type;
    return token;
}

tokenizer_t* tokenizer_init(FILE *in) {
    tokenizer_t *tokenizer = (tokenizer_t*) malloc(sizeof(tokenizer_t));
    tokenizer->in = in;
    tokenizer->cur_line = NULL;
    tokenizer->line_size = getline(&tokenizer->cur_line, &tokenizer->line_size, tokenizer->in);
    tokenizer->line_number = 0;
    tokenizer->index = 0;
    return tokenizer;
}

token_t* tokenizer_read_token(tokenizer_t *tokenizer) {
    if (tokenizer->index == tokenizer->line_size) {
        size_t rv = getline(&tokenizer->cur_line, &tokenizer->line_size, tokenizer->in);
        if (rv == -1) return token_init(NULL, NULL_TOKEN);
        tokenizer->line_size = rv;
        tokenizer->line_number++;
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

        if (rc == PCRE_ERROR_NOMATCH) continue;
        if (rc < 0) {
            printf("PCRE matching error: %d\n", rc);
            exit(1);
        }

        int match_length = ovector[1] - ovector[0];
        tokenizer->index += match_length;
        char *content = strndup(subject, match_length);
        return token_init(content, (token_type_t) i);
    }

    return token_init(tokenizer->cur_line + tokenizer->index, INVALID_TOKEN);
}

int tokenizer_get_line_number(tokenizer_t *tokenizer) { return tokenizer->line_number; }

int tokenizer_get_index(tokenizer_t *tokenizer) { return tokenizer->index; }
