#include "Table.hpp"
#include "Token.hpp"
#include <any>

Table::Table() = default;

int Table::insert(const Row row) {
    if (row.token.type == Token::Id::ID) {
        std::string lex = std::any_cast<std::string>(row.token.attribute);
        if (lexs.contains(lex))
            return -1;
        lexs.insert(lex);
    }

    rows.push_back(row);

    return rows.size();
}
