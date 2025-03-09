#include "SymbolTable.hpp"
#include "Token.hpp"
#include <any>
#include <stdexcept>

SymbolTable::SymbolTable() = default;

SymbolTable::size_type SymbolTable::insert(const Row row) {
    if (row.token.id == Token::Name::ID) {
        std::string lex = std::any_cast<std::string>(row.token.attribute);
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