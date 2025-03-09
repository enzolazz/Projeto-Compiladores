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

char Lexer::look_ahead() const noexcept {
    if (isEOF())
        return '\n';

    if (next_pos == BUFFER_SIZE)
        return buffers[active_buffer ^ 1][0];
    else
        return buffers[active_buffer][next_pos];
}

bool Lexer::isEOF() const noexcept { return eof; }

Token Lexer::next_token() {
    int current_state = 0;
    token = {};
    lexeme = {};

    while (!token.has_value()) {
        char c = next_char();
        char la = look_ahead();

        lexeme += c;

        switch (current_state) {
        case 0:
            current_state = s0_white_space(c, la);
            break;
        case 3:
            current_state = s3_colon(c, la);
            break;
        case 5:
            token = Token(Token::Name::ATTRIBUTION, nullptr, row, col_lex_init);
            break;
        case 90:
            current_state = s90_id_tail(c, la);
            break;
        case 19:
            token = Token(Token::Name::END_SENTENCE, nullptr, row, col_lex_init);
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

int Lexer::s0_white_space(char c, char look_ahead) {
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

int Lexer::s3_colon(char c, char look_ahead) {
    if (c == '=')
        return 5;
    else
        token = Token(Token::Name::COLON, nullptr, row, col_lex_init);
    return 3;
}

int Lexer::s90_id_tail(char c, char look_ahead) {
    if (std::isalnum(c) || c == '_') {
        if (!(std::isalnum(look_ahead) || look_ahead == '_')) {
            token = Token(Token::Name::ID, lexeme, row, col_lex_init);
            symbolTable.insert(Row(token.value()));
        }
        return 90;
    } else
        throw LexerException("Caractere nao reconhecido", row, col, c);
};