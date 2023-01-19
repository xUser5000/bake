//
// Created by xuser5000 on 19/01/23.
//

#include <gtest/gtest.h>

#include "parser/Tokenizer.h"

TEST(TokenizerTest, BasicScriptWithoutWhiteSpaces) {
    std::string test_string = "target:dependency\n\trecipe";
    std::stringstream ss(test_string);
    auto *tokenizer = new Tokenizer(ss);
    Token token;
    token = tokenizer->getNextToken();
    ASSERT_EQ(token.getContent(), "target");
    ASSERT_EQ(token.getType(), IDENTIFIER);

    token = tokenizer->getNextToken();
    ASSERT_EQ(token.getContent(), ":");
    ASSERT_EQ(token.getType(), COLON);

    token = tokenizer->getNextToken();
    ASSERT_EQ(token.getContent(), "dependency");
    ASSERT_EQ(token.getType(), IDENTIFIER);

    token = tokenizer->getNextToken();
    ASSERT_EQ(token.getType(), ENDLINE);

    token = tokenizer->getNextToken();
    ASSERT_EQ(token.getType(), TAB);

    token = tokenizer->getNextToken();
    ASSERT_EQ(token.getContent(), "recipe");
    ASSERT_EQ(token.getType(), IDENTIFIER);

    token = tokenizer->getNextToken();
    ASSERT_EQ(token.getType(), ENDLINE);

    token = tokenizer->getNextToken();
    ASSERT_EQ(token.getType(), INVALID_TOKEN);
}
