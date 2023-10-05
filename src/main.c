#include<stdlib.h>

#include "tokenizer.h"

int main(int argc, char* argv[]) {
    if (argc == 1) {
        printf("bake: No Bakefile provided!\n");
        exit(1);
    }

    if (argc > 2) {
        printf("bake: too many arguments!\n");
        exit(1);
    }

    char *makefile_path = argv[1];
    FILE *makefile = fopen(makefile_path, "r");
    tokenizer_t *tokenizer = tokenizer_init(makefile);
    token_t *token;
    while ((token = tokenizer_read_token(tokenizer)) != NULL) {
        if (token->type == INVALID_TOKEN) {
            printf("Undefined token %s at %d:%d\n",
                    token->content,
                    tokenizer->line_number + 1,
                    tokenizer->index + 1);
            exit(1);
        }
    }
    printf("Bakefile compiled Successfully.\n");

    fclose(makefile);

    return 0;
}
