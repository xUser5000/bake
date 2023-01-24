//
// Created by xuser5000 on 19/01/23.
//

#ifndef BAKE_TOKEN_H
#define BAKE_TOKEN_H

#include <string>
#include <regex>

enum TokenType { INVALID_TOKEN, IDENTIFIER, COLON, ENDLINE, TAB, COMMAND, WHITE_SPACE};

const std::pair<TokenType, std::regex> patterns[] = {
        {IDENTIFIER, std::regex("^[\\w-_]+")},
        {COLON, std::regex("^:")},
        {ENDLINE, std::regex("^\\n")},
        {TAB, std::regex("^\\t")},
        {COMMAND, std::regex("^\".+\"")},
        {WHITE_SPACE, std::regex("^[ ]+")}
};

class Token {
public:
    Token(): type_(INVALID_TOKEN) {}
    Token(const std::string &content, TokenType type) : content_(content), type_(type) {}
    const std::string &getContent() const { return content_; }
    TokenType getType() const { return type_; }
    void setContent(const std::string &content) { content_ = content; }
    void setType(TokenType type) { type_ = type; }

private:
    std::string content_;
    TokenType type_;
};


#endif //BAKE_TOKEN_H
