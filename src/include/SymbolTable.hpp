#pragma once

#include "Token.hpp"
#include <string>
#include <unordered_map>
#include <vector>

struct Row {
    Token token;
    Token::Type coerced_id_type;
    std::any coerced_value;
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

    void insert(Row row);
    size_type get_pos_lexeme(const std::string& lexeme) const;

    const Row& operator[](size_type pos) const;

    Row& operator[](size_type pos);
};
