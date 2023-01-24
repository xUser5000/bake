//
// Created by xuser5000 on 19/01/23.
//

#ifndef BAKE_PARSER_H
#define BAKE_PARSER_H

#include <vector>

#include "parser/Tokenizer.h"
#include "parser/Rule.h"

class Parser {
public:
    Parser(std::istream &in);
    std::vector<Rule> getRules();

private:
    Token consume(TokenType token_type);

    std::vector<Rule> script();
    std::vector<Rule> ruleList();
    Rule rule();
    std::string target();
    std::string colon();
    std::vector<std::string> dependencyList();
    std::string endLine();
    std::string tab();
    std::vector<std::string> recipeList();
    std::string dependency();
    std::string recipe();
    bool optionalWhiteSpace();
    bool optionalLineBreaks();

    Token lookahead_;
    Tokenizer *tokenizer_;
};

#endif //BAKE_PARSER_H
