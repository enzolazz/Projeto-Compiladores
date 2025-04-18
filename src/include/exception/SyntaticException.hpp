#pragma once

#include "Token.hpp"
#include <stdexcept>

class SyntacticException : public std::runtime_error {
 public:
    using size_type = unsigned long;

    Token token;
    const unsigned long row, col;
    const std::string what_s;

    SyntacticException(const std::string &what, const Token &token, unsigned long row, unsigned long col);

    [[nodiscard]] const char *what() const noexcept override;
};
