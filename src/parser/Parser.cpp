//
// Created by xuser5000 on 19/01/23.
//

#include "parser/Parser.h"

Parser::Parser(std::istream &in) { tokenizer_ = new Tokenizer(in); }

std::vector<Rule> Parser::getRules() {
    lookahead_ = tokenizer_->getNextToken();
    return script();
}

Token Parser::consume(TokenType token_type) {
    if (lookahead_.getType() != token_type) {
        throw std::runtime_error("Unexpected Token on line " + std::to_string(tokenizer_->getLineNumber() + 1));
    }
    Token res = lookahead_;
    lookahead_ = tokenizer_->getNextToken();
    return res;
}

std::vector<Rule> Parser::script() {
    optionalLineBreaks();
    return ruleList();
}

std::vector<Rule> Parser::ruleList() {
    std::vector<Rule> rules;
    while (lookahead_.getType() != INVALID_TOKEN) {
        rules.push_back(rule());
        optionalLineBreaks();
    }
    return rules;
}

Rule Parser::rule() {
    Rule r;
    r.setTarget(target());
    optionalWhiteSpace();
    colon();
    optionalWhiteSpace();
    r.setDependencies(dependencyList());
    endLine();
    r.setRecipes(recipeList());
    if (r.getDependencies().empty() && r.getRecipes().empty()) {
        throw std::runtime_error("bake: No dependencies or recipe provided for target" + r.getTarget());
    }
    return r;
}

std::string Parser::target() { return consume(IDENTIFIER).getContent(); }

std::string Parser::colon() { return consume(COLON).getContent(); }

std::vector<std::string> Parser::dependencyList() {
    std::vector<std::string> list;
    while (lookahead_.getType() == IDENTIFIER) {
        list.push_back(dependency());
        optionalWhiteSpace();
    }
    return list;
}

std::string Parser::endLine() { return consume(ENDLINE).getContent(); }

std::string Parser::tab() { return consume(TAB).getContent(); }

std::vector<std::string> Parser::recipeList() {
    std::vector<std::string> list;
    while (lookahead_.getType() == TAB) {
        tab();
        list.push_back(recipe());
        optionalWhiteSpace();
        endLine();
    }
    if (lookahead_.getType() == WHITE_SPACE) {
        throw std::runtime_error("Unexpected Token on line " + std::to_string(tokenizer_->getLineNumber() + 1));
    }
    return list;
}

std::string Parser::dependency() { return consume(IDENTIFIER).getContent(); }

std::string Parser::recipe() {
    std::string command_with_quotes = consume(COMMAND).getContent();
    return command_with_quotes.substr(1, command_with_quotes.size() - 2);
}

bool Parser::optionalWhiteSpace() {
    if (lookahead_.getType() == WHITE_SPACE) {
        consume(WHITE_SPACE);
        return true;
    }
    return false;
}

bool Parser::optionalLineBreaks() {
    bool res = false;
    while (lookahead_.getType() == ENDLINE) {
        res = true;
        consume(ENDLINE);
    }
    return res;
}

