#pragma once

#include "Lexer.hpp"
#include <stdexcept>

class LexerException : public std::runtime_error {
 public:
    const Lexer::size_type row, col;
    const char c;
    const std::string what_s;

    LexerException(std::string what, Lexer::size_type row, Lexer::size_type col,
                   char c);

    const char *what() const noexcept override;
};