//
// Created by xuser5000 on 19/01/23.
//

#ifndef BAKE_PARSER_H
#define BAKE_PARSER_H

#include <vector>

#include "parser/Tokenizer.h"
#include "parser/Rule.h"

/*
 * Script
 *  : Rule
 *  ;
 *
 * Rule
 *  : Target ":" Dependency "\n" "\t" Recipe
 *  ;
 * */

class Parser {
public:
    Parser(std::istream &in);
    std::vector<Rule> getRules();

private:
    Token consume(TokenType token_type);

    Rule script();
    Rule rule();
    std::string target();
    std::string colon();
    std::string dependency();
    std::string endLine();
    std::string tab();
    std::string recipe();

    Token lookahead_;
    Tokenizer *tokenizer_;
};

#endif //BAKE_PARSER_H
