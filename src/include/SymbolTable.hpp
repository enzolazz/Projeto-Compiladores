#pragma once

#include "Token.hpp"
#include <unordered_map>
#include <vector>

struct Row {
    Token token;
    std::string lexeme;
    Token::Type coerced_id_type;
    std::any value;

    Row(const Token &token, const std::string &lexeme);
    Row(const Token &token, signed char lexeme);

    [[nodiscard]] std::string to_string(unsigned long row, unsigned long col) const;
};

class SymbolTable {
    std::vector<Row> rows;

 public:
    using size_type = decltype(rows)::size_type;

 private:
    std::unordered_map<std::string, size_type> lexeme_index;

 public:
    SymbolTable();

    Token &insert(Row row);
    size_type get_pos_lexeme(const std::string &lexeme) const;

    const Row &operator[](size_type pos) const;

    Row &operator[](size_type pos);
};
