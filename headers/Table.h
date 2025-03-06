#ifndef HEADERS_TABLE_H_
#define HEADERS_TABLE_H_

#include "Tokens.h"
#include <string>
#include <vector>
#include <unordered_set>

typedef struct {
    Tokens token;
    std::string lex;
    void *valor;
    void *tipo;
    int row;
    int col;
} Row;


class Table {
private:
    int count;
    std::vector<Row> rows;
    std::unordered_set<std::string> lexs;
public:
    int insert(Row row);

    Table();
};

#endif  // HEADERS_TABLE_H_

