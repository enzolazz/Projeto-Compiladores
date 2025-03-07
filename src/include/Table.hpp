#pragma once

#include "Token.hpp"
#include <string>
#include <unordered_set>
#include <vector>

struct Row {
    Token token;
    std::string lex;
    void *valor;
    void *tipo;
    int row;
    int col;
};

class Table {
 private:
    std::vector<Row> rows;
    std::unordered_set<std::string> lexs;

 public:
    Table();

    int insert(Row row);
};