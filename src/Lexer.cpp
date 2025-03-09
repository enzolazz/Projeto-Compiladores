#include "Lexer.hpp"
#include "SymbolTable.hpp"
#include "Token.hpp"
#include "exception/LexerException.hpp"
#include <cctype>
#include <fstream>

Lexer::Lexer(std::ifstream &source, SymbolTable &symbolTable)
    : source(source), symbolTable(symbolTable), active_buffer(0), row(1), col(1), col_lex_init(1), next_pos(0),
      eofAt(-1) {
    source.read(buffers[0], BUFFER_SIZE);
    if (source.gcount() != BUFFER_SIZE)
        eofAt = source.gcount();
    else {
        source.read(buffers[1], BUFFER_SIZE);
        if (source.gcount() != BUFFER_SIZE)
            eofAt = source.gcount();
    }
}

char Lexer::next_char() {
    if (isEOF())
        return '\n';

    char ret;
    if (next_pos == BUFFER_SIZE) {
        source.read(buffers[active_buffer], BUFFER_SIZE);
        if (source.gcount() != BUFFER_SIZE)
            eofAt = source.gcount();
        active_buffer ^= 1;
        ret = buffers[active_buffer][0];
        next_pos = 1;
    } else {
        ret = buffers[active_buffer][next_pos];
        ++next_pos;
    }

    if (next_pos == eofAt)
        eof = true;

    return ret;
}

void Lexer::look_ahead() noexcept {
    next_pos--;
    col--;
    if (buffers[active_buffer][next_pos] == '\n')
        row--;
}

bool Lexer::isEOF() const noexcept { return eof; }

Token Lexer::next_token() {
    int current_state = 0;
    token = {};
    lexeme = {};

    while (!token.has_value()) {
        char c = next_char();

        lexeme += c;

        switch (current_state) {
        case 0:
            current_state = s0_white_space(c);
            break;
        case 3:
            current_state = s3_colon(c);
            break;
        case 5:
            token = Token(Token::Name::ATTRIBUTION, nullptr, row, col_lex_init);
            break;
        case 19:
            token = Token(Token::Name::END_SENTENCE, nullptr, row, col_lex_init);
            break;
        case 90:
            current_state = s90_id_tail(c);
            break;
        case 91:
            look_ahead();
            token = Token(Token::Name::ID, lexeme, row, col);
            symbolTable.insert(Row(token.value()));
            break;

        default:
            throw LexerException("Transicao nao implementada", row, col, c);
        }

        col++;
        if (c == '\n') {
            row++;
            col = 1;
        }
    }

    return token.value();
}

int Lexer::s0_white_space(char c) {
    if (std::isspace(c)) {
        lexeme.erase(0, 1);
        return 0;
    }
    col_lex_init = col;

    // TODO Este é o primeiro estado do diagrama, após descartar os caracteres white space, fazer a transição
    // para todos outros estados fingindo que estamos lendo o primeiro caractere.

    switch (c) {
    case ':':
        return 3;
    case ';':
        return 19;
    default:
        if ((c >= 'A' && c <= 'Z') || std::string("abghjklmnoqrsuvxyz_").find(c) != std::string::npos)
            return 90;
    }

    throw LexerException("Transicao ainda nao implementada", row, col, c);
};

int Lexer::s3_colon(char c) {
    if (c == '=')
        return 5;
    else
        token = Token(Token::Name::COLON, nullptr, row, col_lex_init);
    return 3;
}

int Lexer::s90_id_tail(char c) {
    if (std::isalnum(c) || c == '_') {
        return 90;
    } else
        return 91;
};