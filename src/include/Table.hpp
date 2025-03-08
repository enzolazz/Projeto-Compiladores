#pragma once

#include "Token.hpp"
#include <string>
#include <unordered_set>
#include <vector>



struct Row {
    Token token;
    void* type; // TODO necessário?
    std::any value;
};

class Table {
 private:
    std::vector<Row> rows;
    std::unordered_set<std::string> lexs;

 public:
    Table();

    int insert(Row row);
};
