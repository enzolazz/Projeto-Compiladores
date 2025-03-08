#include "Lexer.hpp"
#include "Token.hpp"
#include "exception/LexerException.hpp"
#include <fstream>

Lexer::Lexer(std::ifstream &source) : source(source), active_buffer(0), row(1), col(1), col_lex_init(1), next_pos(0) {
    source.read(buffers[0], BUFFER_SIZE);
    source.read(buffers[1], BUFFER_SIZE);

    white_space = [this](char c, char look_ahead) {
        if (std::isspace(c)) {
            lexeme.erase(0, 1);
            return;
        }
        col_lex_init = col;

        // TODO Este é o primeiro estado do diagrama, após descartar os caracteres white space, fazer a transição
        // para todos outros estados fingindo que estamos lendo o primeiro caractere.
        if ((c >= 'A' && c <= 'Z') || std::string("abgjklmnoqrsuvwxyz_").find(c) != std::string::npos)
            current_state = id_tail;
        else
            throw LexerException("Transicao ainda nao implementada", row, col, c);
    };

    current_state = white_space;

    id_tail = [this](char c, char look_ahead) {
        if (std::isalnum(c) || c == '_') {
            if (!(std::isalnum(look_ahead) || look_ahead == '_'))
                token = Token(Token::Name::ID, lexeme, row, col_lex_init);
        } else
            throw LexerException("Caractere nao reconhecido", row, col, c);
    };
}

char Lexer::next_char() {
    char ret;
    if (next_pos == BUFFER_SIZE) {
        source.read(buffers[active_buffer], BUFFER_SIZE);
        active_buffer ^= 1;
        ret = buffers[active_buffer][0];
        next_pos = 1;
    } else {
        ret = buffers[active_buffer][next_pos];
        ++next_pos;
    }

    return ret;
}

char Lexer::look_ahead() const noexcept {
    if (next_pos == BUFFER_SIZE)
        return buffers[active_buffer ^ 1][0];
    else
        return buffers[active_buffer][next_pos];
}

Token Lexer::next_token() {
    current_state = white_space;
    token = {};
    lexeme = {};

    while (!token.has_value()) {
        char c = next_char();
        char la = look_ahead();

        lexeme += c;
        current_state(c, la);
        col++;
        if (c == '\n') {
            row++;
            col = 1;
        }
    }

    return token.value();
}
