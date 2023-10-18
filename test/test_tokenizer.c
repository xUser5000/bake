#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#include "utest.h"
#include "tokenizer.h"

UTEST_MAIN();

#define ASSERT_TOKEN_EQ(tokenizer, token_content, token_type) { \
    token_t *token = tokenizer_read_token(tokenizer); \
    ASSERT_STREQ(token->content, token_content); \
    ASSERT_EQ(token->type, token_type); }

#define ASSERT_TOKEN_NULL(tokenizer) {                          \
    token_t *token = tokenizer_read_token(tokenizer);           \
    ASSERT_EQ(token->content, NULL);                            \
    ASSERT_EQ(token->type, NULL_TOKEN);                         \
}


UTEST(Tokenizer, BasicScriptWithoutWhiteSpaces) {
    char *test_string = "target:dependency\n\t\"recipe\"";
    FILE *ss = fmemopen(test_string, strlen(test_string), "r");
    tokenizer_t *tokenizer = tokenizer_init(ss);
    
    ASSERT_TOKEN_EQ(tokenizer, "target", IDENTIFIER);
    ASSERT_TOKEN_EQ(tokenizer, ":", COLON);
    ASSERT_TOKEN_EQ(tokenizer, "dependency", IDENTIFIER);
    ASSERT_TOKEN_EQ(tokenizer, "\n", ENDLINE);
    ASSERT_TOKEN_EQ(tokenizer, "\t", TAB);
    ASSERT_TOKEN_EQ(tokenizer, "\"recipe\"", COMMAND);
    ASSERT_TOKEN_EQ(tokenizer, "\n", ENDLINE);
    ASSERT_TOKEN_NULL(tokenizer);
}

UTEST(Tokenizer, BasicScriptWithWhiteSpaces) {
    char *test_string = "target  :dependency     \n\t\"recipe\"";
    FILE *ss = fmemopen(test_string, strlen(test_string), "r");
    tokenizer_t *tokenizer = tokenizer_init(ss);

    ASSERT_TOKEN_EQ(tokenizer, "target", IDENTIFIER);
    ASSERT_TOKEN_EQ(tokenizer, "  ", WHITE_SPACE);
    ASSERT_TOKEN_EQ(tokenizer, ":", COLON);
    ASSERT_TOKEN_EQ(tokenizer, "dependency", IDENTIFIER);
    ASSERT_TOKEN_EQ(tokenizer, "     ", WHITE_SPACE);
    ASSERT_TOKEN_EQ(tokenizer, "\n", ENDLINE);
    ASSERT_TOKEN_EQ(tokenizer, "\t", TAB);
    ASSERT_TOKEN_EQ(tokenizer, "\"recipe\"", COMMAND);
    ASSERT_TOKEN_EQ(tokenizer, "\n", ENDLINE);
    ASSERT_TOKEN_NULL(tokenizer);
}

UTEST(Tokenizer, IgnoreEmptyLines) {
  char *test_string = "target  :dependency     \n\t\n     \n\t\t\t\t\t";
  FILE *ss = fmemopen(test_string, strlen(test_string), "r");
  tokenizer_t *tokenizer = tokenizer_init(ss);

  ASSERT_TOKEN_EQ(tokenizer, "target", IDENTIFIER);
  ASSERT_TOKEN_EQ(tokenizer, "  ", WHITE_SPACE);
  ASSERT_TOKEN_EQ(tokenizer, ":", COLON);
  ASSERT_TOKEN_EQ(tokenizer, "dependency", IDENTIFIER);
  ASSERT_TOKEN_EQ(tokenizer, "     ", WHITE_SPACE);
  ASSERT_TOKEN_EQ(tokenizer, "\n", ENDLINE);
  ASSERT_TOKEN_EQ(tokenizer, "\n", ENDLINE);
  ASSERT_TOKEN_NULL(tokenizer);
}
