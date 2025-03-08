#include "exception/LexerException.hpp"
#include <stdexcept>
#include <string>

LexerException::LexerException(std::string what, Lexer::size_type row,
                               Lexer::size_type col, char c)
    : std::runtime_error(what), row(row), col(col), c(c),

      what_s(std::string("Char ") + c + " at row " + std::to_string(row) +
             " col " + std::to_string(col) + ": " +
             std::runtime_error::what()) {}

const char *LexerException::what() const noexcept { return what_s.c_str(); }