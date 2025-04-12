#pragma once

#include "Lexer.hpp"
#include <stdexcept>

class LexerException : public std::runtime_error {
 public:
    const Token::size_type row, col;
    const signed char c;
    const std::string what_s;

    LexerException(const std::string& what, Token::size_type row, Token::size_type col, signed char c);

    const char *what() const noexcept override;
};