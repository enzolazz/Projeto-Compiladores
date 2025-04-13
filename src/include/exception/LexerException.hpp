#pragma once

#include <stdexcept>

class LexerException : public std::runtime_error {
 public:
    const unsigned long row, col;
    const signed char c;
    const std::string what_s;

    LexerException(const std::string &what, unsigned long row, unsigned long col, signed char c);

    const char *what() const noexcept override;
};
