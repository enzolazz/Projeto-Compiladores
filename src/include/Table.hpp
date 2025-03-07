#pragma once

#include "Token.hpp"
#include <string>
#include <unordered_set>
#include <vector>

enum class Type : short { INT, FLOAT, CHAR };

struct Row {
    Token token;
    Type type;
    union {
        int INT;
        float FLOAT;
        char CHAR;
    } value;
};

class Table {
 private:
    std::vector<Row> rows;
    std::unordered_set<std::string> lexs;

 public:
    Table();

    int insert(Row row);
    void print();
};
