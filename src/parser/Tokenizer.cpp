//
// Created by xuser5000 on 19/01/23.
//
#include "parser/Tokenizer.h"

Tokenizer::Tokenizer(std::istream &in): in_(in) {
    std::getline(in_, cur_line_);
    cur_line_ += '\n';
    line_number_ = 0;
    index_ = 0;
}

Token Tokenizer::getNextToken() {
    if (index_ == (int) cur_line_.size()) {
        if (!std::getline(in_, cur_line_)) {
            return Token();
        }
        cur_line_ += '\n';
        index_ = 0;
        line_number_++;
    }
    std::string s = cur_line_.substr(index_);

    std::smatch matches;
    for (const auto& p: patterns) {
        if (std::regex_search(s, matches, p.second)) {
            index_ += (int) matches[0].length();
            return Token(matches[0], p.first);
        }
    }

    return Token();
}

int Tokenizer::getLineNumber() {
    return line_number_;
}
