//
// Created by xuser5000 on 19/01/23.
//

#include "parser/Parser.h"

Parser::Parser(std::istream &in) { tokenizer_ = new Tokenizer(in); }

std::vector<Rule> Parser::getRules() {
    lookahead_ = tokenizer_->getNextToken();
    std::vector<Rule> rules;
    rules.push_back(script());
    return rules;
}

Token Parser::consume(TokenType token_type) {
    if (lookahead_.getType() != token_type) {
        throw std::runtime_error("Unexpected Token on line " + std::to_string(tokenizer_->getLineNumber()));
    }
    Token res = lookahead_;
    lookahead_ = tokenizer_->getNextToken();
    return res;
}

Rule Parser::script() {
    return rule();
}

Rule Parser::rule() {
    Rule r;
    r.setTarget(target());
    colon();
    r.setDependency(dependency());
    endLine();
    tab();
    r.setRecipe(recipe());
    return r;
}

std::string Parser::target() { return consume(IDENTIFIER).getContent(); }

std::string Parser::colon() { return consume(COLON).getContent(); }

std::string Parser::dependency() { return consume(IDENTIFIER).getContent(); }

std::string Parser::endLine() { return consume(ENDLINE).getContent(); }

std::string Parser::tab() { return consume(TAB).getContent(); }

std::string Parser::recipe() { return consume(IDENTIFIER).getContent(); }
