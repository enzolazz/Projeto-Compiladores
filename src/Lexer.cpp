#include "Lexer.hpp"
#include "Token.hpp"
#include "debug.hpp"
#include <fstream>

Lexer::Lexer(std::ifstream &source)
    : source(source), active_buffer(0), row(1), col(1), next_pos(0) {
    source.read(buffers[0], BUFFER_SIZE);
    source.read(buffers[1], BUFFER_SIZE);
}

char Lexer::next_char() {
    char ret;
    if (next_pos == BUFFER_SIZE - 1) {
        source.read(buffers[active_buffer], BUFFER_SIZE);
        active_buffer ^= active_buffer;
        ret = buffers[active_buffer][0];
        next_pos = 1;
    } else {
        ret = buffers[active_buffer][next_pos];
        ++next_pos;
    }

    return ret;
}

char Lexer::lookAhead() const noexcept {
    if (next_pos == BUFFER_SIZE - 1)
        return buffers[active_buffer ^ active_buffer][0];
    else
        return buffers[active_buffer][next_pos];
}

Token Lexer::next_token() {
    std::string lex;
    bool found = false;

    int state = 0;
    while (!found) {
        char c = next_char();
        lex += c;
        col++;
        db_file("Lendo caractere", c, state, row, col);

        switch (state) {
        case 0:
            state = (c == 'w') ? 1 : -1;
            break;
        case 1:
            state = (c == 'h') ? 2 : -1;
            break;
        case 2:
            state = (c == 'i') ? 3 : -1;
            break;
        case 3:
            state = (c == 'l') ? 4 : -1;
            break;
        case 4:
            if (c == 'e') {
                char ahead = lookAhead();
                if (!std::isalnum(ahead) && ahead != '_')
                    return Token(Token::Id::WHILE, nullptr, row, col);
            } else
                state = -1;
            break;
        case -1:
            if (c == ' ' || c == '\0')
                col++;
            else if (c == '\t')
                col += 4; // TODO tamanho do TAB varia de acordo o editor de
                          // texto, nem sempre são 4 colunas.
            else if (c == '\n') {
                col = 0;
                row++;
            } else {
                db_file("Caractere inesperado", c, row, col);
                throw 505;
            }
        default:
            break;
        }
    }

    return {}; // TODO retornar Token?
}
