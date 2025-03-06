#include "../headers/Table.h"
#include <cstdlib>


Table::Table() {
    count = 0;
}

int Table::insert(const Row row) {
    if (lexs.find(row.lex) != lexs.end()) return -1;

    rows.push_back(row);
    lexs.insert(row.lex);

    return count++;
}
