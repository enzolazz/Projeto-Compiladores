#include "SymbolTable.hpp"
#include <stdexcept>

SymbolTable::SymbolTable() = default;

Row::Row(const Token& token, const std::string& lexeme) : token(token), lexeme(lexeme) {
    if (token.name == Token::Name::CONST) {
        if (lexeme[0] == '\'') {
            value = lexeme;
            coerced_id_type = Token::Type::CHAR;
        } else if (lexeme.find('.') != std::string::npos || lexeme.find('E') != std::string::npos) {
            value = std::stof(lexeme);
            coerced_id_type = Token::Type::FLOAT;
        } else {
            value = std::stoi(lexeme);
            coerced_id_type = Token::Type::INT;
        }
    }
}

Row::Row(const Token& token, const signed char lexeme) : Row(token, std::string(1, lexeme)) {}

std::string Row::to_string() const {
    std::string s = "Lexeme: " + lexeme + "; Token: " + token.to_string();

    if (token.name == Token::Name::CONST) {
        s += "; CoercedIdType: " + Token::to_string(coerced_id_type) + "; Value: ";
        switch (coerced_id_type) {
        case Token::Type::FLOAT:
            s += std::to_string(std::any_cast<float>(value));
            break;
        case Token::Type::CHAR:
            s += std::any_cast<std::string>(value);
            break;
        case Token::Type::INT:
            s += std::to_string(std::any_cast<int>(value));
            break;
        }
    }

    return s;
}

Token SymbolTable::insert(Row row) {
    if (row.token.name == Token::Name::ID || row.token.name == Token::Name::CONST) {
        const auto lex = std::any_cast<std::string>(row.lexeme);
        try {
            const size_type index = lexeme_index.at(lex);
            return operator[](index).token;
        } catch (std::out_of_range &) {
            const size_type index = rows.size();
            lexeme_index[lex] = index;
            row.token.attribute = index;
        }
    }

    rows.push_back(row);
    return row.token;
}

SymbolTable::size_type SymbolTable::get_pos_lexeme(const std::string &lexeme) const { return lexeme_index.at(lexeme); }

const Row &SymbolTable::operator[](const size_type pos) const { return rows[pos]; }

Row &SymbolTable::operator[](const size_type pos) { return rows[pos]; }