#pragma once

#include "Token.hpp"
#include <fstream>
#include <functional>
#include <optional>

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

    std::optional<Token> token;
    std::string lexeme;
    std::function<void(char, char)> current_state, white_space, id_tail;

    char next_char();
    char look_ahead() const noexcept;

 public:
    Lexer(std::ifstream &source);
    Token next_token();
};
