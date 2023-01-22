//
// Created by xuser5000 on 19/01/23.
//

#include <gtest/gtest.h>

#include "parser/Parser.h"

TEST(ParserTest, BasicScriptWithoutWhiteSpaces) {
    std::string test_string = "target:dependency\n\t\"recipe\"";
    std::stringstream ss(test_string);

    auto parser = new Parser(ss);
    Rule rule = parser->getRules()[0];

    ASSERT_EQ(rule.getTarget(), "target");
    ASSERT_EQ(rule.getDependencies()[0], "dependency");
    ASSERT_EQ(rule.getRecipes()[0], "recipe");
}

TEST(ParserTest, BasicScriptWithWhiteSpaces) {
    std::string test_string = "target  :dependency     \n\t\"recipe\"";
    std::stringstream ss(test_string);

    auto parser = new Parser(ss);
    Rule rule = parser->getRules()[0];

    ASSERT_EQ(rule.getTarget(), "target");
    ASSERT_EQ(rule.getDependencies()[0], "dependency");
    ASSERT_EQ(rule.getRecipes()[0], "recipe");
}

TEST(TokenizerTest, MultipleDependenciesAndMultipleRecipes) {
    std::string test_string = "target  :dependency1 dependency2    \n\t\"recipe1\" \n\t\"recipe2\"";
    std::stringstream ss(test_string);

    auto parser = new Parser(ss);
    Rule rule = parser->getRules()[0];

    ASSERT_EQ(rule.getTarget(), "target");
    ASSERT_EQ(rule.getDependencies(), std::vector<std::string>({"dependency1", "dependency2"}));
    ASSERT_EQ(rule.getRecipes(), std::vector<std::string>({"recipe1", "recipe2"}));
}

TEST(ParserTest, InvalidScript) {
    auto ss = std::stringstream(" target  :dependency1 dependency2    \n\t\"recipe1\" \n\t\"recipe2\"");
    auto parser = new Parser(ss);
    ASSERT_THROW(parser->getRules(), std::runtime_error);

    ss = std::stringstream("target  :dependency1 dependency2    \n \t\"recipe1\" \n\t\"recipe2\"");
    parser = new Parser(ss);
    ASSERT_THROW(parser->getRules(), std::runtime_error);

}
