#pragma once

#include "Token.hpp"
#include <any>
#include <string>
#include <unordered_map>
#include <vector>

struct Row {
    const Token &token;
    std::string lexeme;
    Token::Type coerced_id_type;
    std::any value;

    Row(Token &token, std::string lexeme);
    Row(Token &token, signed char lexeme);

    std::string to_string() const;
};

class SymbolTable {
 private:
    std::vector<Row> rows;

 public:
    using size_type = decltype(rows)::size_type;

 private:
    std::unordered_map<std::string, size_type> lexeme_index;

 public:
    SymbolTable();

    size_type insert(const Row row);
    size_type get_pos_lexeme(const std::string &lexeme) const;

    const Row &operator[](size_type pos) const;

    Row &operator[](size_type pos);
};
