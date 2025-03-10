#pragma once

#include "Token.hpp"
#include <any>
#include <functional>
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

    template <typename T> std::string to_string(bool print_token, std::function<std::string(T)> to_str) const {
        return (print_token ? token.to_string() : "") + "lexeme: " + lexeme +
               "coerced_id_type: " + Token::to_string(coerced_id_type) + ", value: " + to_str(std::any_cast<T>(value));
    }

    template <typename T> std::string to_string(bool print_token) const {
        return to_string<T>(print_token, std::to_string);
    }

    template <> std::string to_string<std::string>(bool print_token) const {
        return to_string<std::string>(print_token, std::identity());
    }

    template <> std::string to_string<signed char>(bool print_token) const {
        return to_string<signed char>(print_token, [](auto c) { return std::string(1, c); });
    }
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
