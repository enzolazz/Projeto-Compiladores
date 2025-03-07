#include "Table.hpp"
#include "Token.hpp"
#include <iostream>

Table::Table() = default;

int Table::insert(const Row row) {
    if (row.token.name == TokenName::ID) {
        std::string *lexPtr = static_cast<std::string *>(row.token.attribute);
        if (lexs.contains(*lexPtr))
            return -1;
        lexs.insert(*lexPtr);
    }

    rows.push_back(row);

    return rows.size();
}
