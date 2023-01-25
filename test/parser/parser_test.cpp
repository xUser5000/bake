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

TEST(ParserTest, MultipleDependenciesAndMultipleRecipes) {
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

TEST(ParserTest, MultipleRules) {
    Rule r1("target1", std::vector<std::string>({"dependency1"}), std::vector<std::string>({"recipe1"}));
    Rule r2("target2", std::vector<std::string>({"dependency2"}), std::vector<std::string>({"recipe2", "recipe3"}));

    std::string s1 = "target1:dependency1\n\t\"recipe1\"\n";
    std::string s2 = "target2:dependency2\n\t\"recipe2\"\n\t\"recipe3\"";
    std::stringstream ss(s1 + s2);
    auto parser = new Parser(ss);
    std::vector<Rule> res = parser->getRules();
    ASSERT_EQ((int) res.size(), 2);
    ASSERT_EQ(res[0], r1);
    ASSERT_EQ(res[1], r2);

    s1 = "\n\ntarget1:dependency1\n\t\"recipe1\"\n\n\n";
    s2 = "target2:dependency2\n\t\"recipe2\"\n\t\"recipe3\"\n\n";
    ss = std::stringstream(s1 + s2);
    parser = new Parser(ss);
    res = parser->getRules();
    ASSERT_EQ((int) res.size(), 2);
    ASSERT_EQ(res[0], r1);
    ASSERT_EQ(res[1], r2);
}

TEST(ParserTest, NoDependencies) {
    std::stringstream ss("target : \n\t\"recipe1\"\n\t\"recipe2\"");
    Parser parser(ss);
    std::vector<Rule> res = parser.getRules();
    ASSERT_EQ(res[0].getTarget(), "target");
    ASSERT_TRUE(res[0].getDependencies().empty());
    ASSERT_EQ(res[0].getRecipes(), std::vector<std::string>({"recipe1", "recipe2"}));
}

TEST(ParserTest, NoRecipes) {
    std::stringstream ss("target : dependency1 dependency2 \n");
    Parser parser(ss);
    std::vector<Rule> res = parser.getRules();
    ASSERT_EQ(res[0].getTarget(), "target");
    ASSERT_EQ(res[0].getDependencies(), std::vector<std::string>({"dependency1", "dependency2"}));
    ASSERT_TRUE(res[0].getRecipes().empty());
}

TEST(ParserTest, NoDependenciesNoRules) {
    std::stringstream ss("target : ");
    Parser parser(ss);
    ASSERT_THROW(parser.getRules(), std::runtime_error);
}
