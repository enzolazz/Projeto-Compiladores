#pragma once

#include "SymbolTable.hpp"
#include "Token.hpp"
#include <fstream>
#include <optional>

class Lexer {
 public:
    using size_type = unsigned long;

 private:
    static constexpr int BUFFER_SIZE = 128;
    std::ifstream &source;
    SymbolTable &symbolTable;
    char buffers[2][BUFFER_SIZE];
    unsigned short active_buffer;
    size_type row, col, col_lex_init;
    int next_pos;

    bool eof;
    int eofAt;

    std::optional<Token> token;
    std::string lexeme;

    char next_char();
    char look_ahead() const noexcept;

    using state_type = int(char, char);
    state_type s0_white_space, s3_colon, s90_id_tail;

 public:
    Lexer(std::ifstream &source, SymbolTable &symbolTable);
    Token next_token();
    bool isEOF() const noexcept;
};
