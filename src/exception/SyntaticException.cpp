#include "exception/SyntaticException.hpp"
#include <stdexcept>
#include <string>

SyntacticException::SyntacticException(const std::string &what, const Token token, const unsigned long row,
                                       const unsigned long col)
    : std::runtime_error(what), token(token), row(row), col(col),

      what_s(std::string("Unexpected token: ") + token.to_string(row, col) + " at row " + std::to_string(row) +
             " col " + std::to_string(col) + ": " + std::runtime_error::what()) {}

const char *SyntacticException::what() const noexcept { return what_s.c_str(); }
