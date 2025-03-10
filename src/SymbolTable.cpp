#include "SymbolTable.hpp"
#include "Token.hpp"
#include <any>
#include <cstdlib>
#include <stdexcept>
#include <string>

SymbolTable::SymbolTable() = default;

Row::Row(Token &token, std::string lexeme) : token(token), lexeme(lexeme) {
    if (token.name == Token::Name::NUM) {
        if (lexeme.find('.') != std::string::npos || lexeme.find('E') != std::string::npos) {
            value = std::stof(lexeme);
            coerced_id_type = Token::Type::FLOAT;
        } else {
            value = std::stoi(lexeme);
            coerced_id_type = Token::Type::INT;
        }
    } else if (token.name == Token::Name::CARACTERE) {
        coerced_id_type = Token::Type::CHAR;
    }
}

Row::Row(Token &token, signed char lexeme) : Row(token, std::string(1, lexeme)) {}

std::string Row::to_string() const {
    auto ret = "lexeme: " + lexeme;
    if (token.name == Token::Name::NUM)
        ret += " type: " + Token::to_string(coerced_id_type) + " value: " +
               (coerced_id_type == Token::Type::INT ? std::to_string(std::any_cast<int>(value))
                                                    : std::to_string(std::any_cast<float>(value)));
    else if (token.name == Token::Name::CARACTERE)
        ret += " type: " + Token::to_string(coerced_id_type);

    return ret;
}

SymbolTable::size_type SymbolTable::insert(const Row row) {
    if (row.token.name == Token::Name::ID) {
        const std::string lex = std::any_cast<std::string>(row.lexeme);
        try {
            return lexeme_index.at(lex);
        } catch (std::out_of_range &) {
            auto index = rows.size();
            lexeme_index[lex] = index;
        }
    }

    rows.push_back(row);
    return rows.size() - 1;
}

SymbolTable::size_type SymbolTable::get_pos_lexeme(const std::string &lexeme) const { return lexeme_index.at(lexeme); }

const Row &SymbolTable::operator[](size_type pos) const { return rows[pos]; }

Row &SymbolTable::operator[](size_type pos) { return rows[pos]; }