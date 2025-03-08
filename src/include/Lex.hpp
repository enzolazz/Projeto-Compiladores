#pragma once

#include "Token.hpp"
#include <fstream>

constexpr int BUFFER_SIZE = 100;

class Lex {
 private:
    std::ifstream& source;
    char buffer1[BUFFER_SIZE];
    char buffer2[BUFFER_SIZE];
    int row, col;
    char c;
    int pos;

    char next_char();
    void lookAhead();

 public:
    Lex(std::ifstream& source);
    Token next_token();
};
