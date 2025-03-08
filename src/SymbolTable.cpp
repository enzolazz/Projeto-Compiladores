#include "SymbolTable.hpp"
#include "Token.hpp"
#include <any>

SymbolTable::SymbolTable() = default;

void SymbolTable::insert(const Row row) {
    if (row.token.id == Token::Name::ID) {
        std::string lex = std::any_cast<std::string>(row.token.attribute);
        if (lexeme_index.contains(lex))
            return;
        auto index = rows.size();
        lexeme_index[lex] = index;
    }

    rows.push_back(row);
}

SymbolTable::size_type
SymbolTable::get_pos_lexeme(const std::string &lexeme) const {
    return lexeme_index.at(lexeme);
}

const Row &SymbolTable::operator[](size_type pos) const { return rows[pos]; }

Row &SymbolTable::operator[](size_type pos) { return rows[pos]; }