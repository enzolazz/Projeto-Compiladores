#include "exception/LexerException.hpp"
#include <stdexcept>
#include <string>

static std::string translate_char(const signed char c) {
    switch (c) {
    case -1:
        return "EOF";
    case '\n':
        return "\\n";
    default:
        return {1, c};
    }
}

LexerException::LexerException(const std::string &what, const unsigned long row, const unsigned long col,
                               const signed char c)
    : std::runtime_error(what), row(row), col(col), c(c),

      what_s(std::string("Char ") + translate_char(c) + " at row " + std::to_string(row) + " col " +
             std::to_string(col) + ": " + std::runtime_error::what()) {}

const char *LexerException::what() const noexcept { return what_s.c_str(); }
