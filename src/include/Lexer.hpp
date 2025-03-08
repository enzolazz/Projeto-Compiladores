#pragma once

#include "Token.hpp"
#include <fstream>

constexpr int BUFFER_SIZE = 128;

class Lexer {
 public:
    using size_type = unsigned long;

 private:
    std::ifstream &source;
    char buffers[2][BUFFER_SIZE];
    unsigned short active_buffer;
    size_type row, col;
    size_type next_pos;

    char next_char();
    char lookAhead() const noexcept;

 public:
    Lexer(std::ifstream &source);
    Token next_token();
};
