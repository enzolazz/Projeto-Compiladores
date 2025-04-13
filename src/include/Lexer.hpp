#pragma once

#include "SymbolTable.hpp"
#include "Token.hpp"
#include <fstream>
#include <optional>

class Lexer {
    static constexpr int BUFFER_SIZE = 128;
    std::ifstream &source;
    char buffers[2][BUFFER_SIZE];
    unsigned short active_buffer;
    unsigned long row, col, col_lex_init;
    int next_pos;
    bool eof;

    std::optional<Token> token;
    std::string lexeme;
    signed char c;

    signed char next_char();
    void look_ahead();

    using state_type = int(signed char);
    state_type s0_inicio_token, s20_num, s90_id_tail;

 public:
    explicit Lexer(std::ifstream &source);
    std::tuple<Token, unsigned long, unsigned long> next_token();
    SymbolTable symbolTable;
};
