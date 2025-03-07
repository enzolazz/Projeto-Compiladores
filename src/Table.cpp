#include "Table.hpp"

Table::Table() = default;

int Table::insert(const Row row) {
    if (lexs.contains(row.lex))
        return -1;

    rows.push_back(row);
    lexs.insert(row.lex);

    return rows.size();
}