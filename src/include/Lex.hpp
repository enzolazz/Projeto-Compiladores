#pragma once

#include "Table.hpp"

#define BUFFER_SIZE 100

class Lex {
 private:
    FILE *source;
    char buffer1[BUFFER_SIZE];
    char buffer2[BUFFER_SIZE];
    int row, col;
    char c;
    int pos;

    char next_char();
    void lookAhead();

 public:
    Lex();
    Lex(FILE *source);
    Token next_token();
};
