//
// Created by xuser5000 on 19/01/23.
//

#ifndef BAKE_TOKENIZER_H
#define BAKE_TOKENIZER_H

#include <istream>

#include "parser/Token.h"

class Tokenizer {
public:
    explicit Tokenizer(std::istream &in);
    Token getNextToken();
    int getLineNumber();

private:
    std::istream &in_;
    std::string cur_line_;
    int line_number_;
    int index_;
};


#endif //BAKE_TOKENIZER_H
