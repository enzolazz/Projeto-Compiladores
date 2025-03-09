#include "Lexer.hpp"
#include "SymbolTable.hpp"
#include "Token.hpp"
#include "exception/LexerException.hpp"
#include <cctype>
#include <fstream>
#include <stdexcept>
#include <string>

Lexer::Lexer(std::ifstream &source, SymbolTable &symbolTable)
    : source(source), symbolTable(symbolTable), active_buffer(0), row(1), col(1), col_lex_init(1), next_pos(0),
      eofAt({-1, -1}) {
    source.read(buffers[0], BUFFER_SIZE);
    if (source.gcount() != BUFFER_SIZE)
        eofAt = {0, source.gcount()};
    else {
        source.read(buffers[1], BUFFER_SIZE);
        if (source.gcount() != BUFFER_SIZE)
            eofAt = {1, source.gcount()};
    }
}

char Lexer::next_char() {
    if (isEOF()) [[unlikely]]
        return '\n';

    char ret;
    if (next_pos == BUFFER_SIZE) {
        source.read(buffers[active_buffer], BUFFER_SIZE);
        if (source.gcount() != BUFFER_SIZE && std::get<0>(eofAt) < 0)
            eofAt = {active_buffer, source.gcount()};
        active_buffer ^= 1;
        ret = buffers[active_buffer][0];
        next_pos = 1;
    } else {
        ret = buffers[active_buffer][next_pos];
        ++next_pos;
    }

    if (next_pos == std::get<1>(eofAt) && active_buffer == std::get<0>(eofAt))
        eof = true;

    return ret;
}

void Lexer::look_ahead() {
    if (lexeme.empty()) [[unlikely]]
        throw std::logic_error("Look ahead utilizado quando lexema era vazio. Nao ha nada para reverter");

    next_pos--;
    col--;
    if (buffers[active_buffer][next_pos] == '\n')
        row--;
    lexeme.pop_back();
}

bool Lexer::isEOF() const noexcept { return eof; }

static bool isValidIdChar(char c) { return std::isalnum(c) || c == '_'; }

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
            token = Token(Token::Name::ATTRIBUITION, nullptr, row, col_lex_init);
            break;
        case 6:
            if (c == '}')
                token = Token(Token::Name::BLOCO_END, nullptr, row, col_lex_init);
            else
                throw LexerException("Caractere não reconhecido", row, col, c);
            break;
        case 8:
            if (c == '%')
                token = Token(Token::Name::BLOCO_START, nullptr, row, col_lex_init);
            else if (c == '#')
                current_state = 10;
            else
                throw LexerException("Caractere não reconhecido", row, col, c);
            break;
        case 10:
            if (c == '#')
                current_state = 11;
            else
                current_state = 10;
            break;
        case 11:
            if (c == '}') {
                current_state = 0;
                lexeme = {};
            }
            break;
        case 15:
            current_state = 16;
            break;
        case 16:
            if (c == '\'') {
                token = Token(Token::Name::CARACTERE, lexeme[1], row, col_lex_init);
                symbolTable.insert(Row(token.value()));
            } else
                throw LexerException("Caractere não reconhecido", row, col, c);
            break;
        case 20:
            current_state = s20_num(c);
            break;
        case 21:
            if (c >= '0' && c <= '9')
                current_state = 22;
            else
                throw LexerException("Caractere não reconhecido", row, col, c);
            break;
        case 22:
            if (c >= '0' && c <= '9')
                current_state = 22;
            else if (c == 'E')
                current_state = 23;
            else {
                current_state = s26_num_f(0);
            }
            break;
        case 23:
            if (c == '+' || c == '-')
                current_state = 24;
            else if (c >= '0' && c <= '9')
                current_state = 25;
            else
                throw LexerException("Caractere não reconhecido", row, col, c);
            break;
        case 24:
            if (c >= '0' && c <= '9')
                current_state = 25;
            else
                throw LexerException("Caractere não reconhecido", row, col, c);
            break;
        case 25:
            if (c >= '0' && c <= '9')
                current_state = 25;
            else
                current_state = s26_num_f(0);
            break;
        case 28:
            if (c == '*')
                token = Token(Token::Name::OPPOT, nullptr, row, col_lex_init);
            else {
                look_ahead();
                token = Token(Token::Name::OPMULDIV, Token::OpMulDiv::MUL, row, col_lex_init);
            }
            break;
        case 35:
            switch (c) {
            case '=':
                token = Token(Token::Name::RELOP, Token::RelOp::LE, row, col_lex_init);
                break;
            case '>':
                token = Token(Token::Name::RELOP, Token::RelOp::NE, row, col_lex_init);
                break;
            default:
                look_ahead();
                token = Token(Token::Name::RELOP, Token::RelOp::LT, row, col_lex_init);
            }
            break;
        case 39:
            if (c == '=')
                token = Token(Token::Name::RELOP, Token::RelOp::GE, row, col_lex_init);
            else
                token = Token(Token::Name::RELOP, Token::RelOp::GT, row, col_lex_init);
            break;
        case 42:
            if (c == 'o')
                current_state = 43;
            else {
                look_ahead();
                current_state = 90;
            }
            break;
        case 43:
            if (isValidIdChar(c))
                current_state = 90;
            else {
                look_ahead();
                token = Token(Token::Name::DO, nullptr, row, col_lex_init);
            }
            break;
        case 45:
            if (c == 'l')
                current_state = 46;
            else {
                look_ahead();
                current_state = 90;
            }
            break;
        case 46:
            if (c == 's')
                current_state = 47;
            else {
                look_ahead();
                current_state = 90;
            }
            break;
        case 47:
            if (c == 'e')
                current_state = 48;
            else {
                look_ahead();
                current_state = 90;
            }
            break;
        case 48:
            if (c == 'i')
                current_state = 50;
            else if (isValidIdChar(c))
                current_state = 90;
            else {
                look_ahead();
                token = Token(Token::Name::ELSE, nullptr, row, col_lex_init);
            }
            break;
        case 50:
            if (c == 'f')
                current_state = 51;
            else {
                look_ahead();
                current_state = 90;
            }
            break;
        case 51:
            if (isValidIdChar(c))
                current_state = 90;
            else {
                look_ahead();
                token = Token(Token::Name::ELSEIF, nullptr, row, col_lex_init);
            }
            break;
        case 53:
            if (c == 'f')
                current_state = 54;
            else if (c == 'n')
                current_state = 56;
            else {
                look_ahead();
                current_state = 90;
            }
            break;
        case 54:
            if (isValidIdChar(c))
                current_state = 90;
            else {
                look_ahead();
                token = Token(Token::Name::IF, nullptr, row, col_lex_init);
            }
            break;
        case 56:
            if (c == 't')
                current_state = 57;
            else {
                look_ahead();
                current_state = 90;
            }
            break;
        case 57:
            if (isValidIdChar(c))
                current_state = 90;
            else {
                look_ahead();
                token = Token(Token::Name::TYPE, Token::Type::INT, row, col_lex_init);
            }
            break;
        case 59:
            if (c == 'l')
                current_state = 60;
            else {
                look_ahead();
                current_state = 90;
            }
            break;
        case 60:
            if (c == 'o')
                current_state = 61;
            else {
                look_ahead();
                current_state = 90;
            }
            break;
        case 61:
            if (c == 'a')
                current_state = 62;
            else {
                look_ahead();
                current_state = 90;
            }
            break;
        case 62:
            if (c == 't')
                current_state = 63;
            else {
                look_ahead();
                current_state = 90;
            }
            break;
        case 63:
            if (isValidIdChar(c))
                current_state = 90;
            else {
                look_ahead();
                token = Token(Token::Name::TYPE, Token::Type::FLOAT, row, col_lex_init);
            }
            break;
        case 65:
            if (c == 'h')
                current_state = 66;
            else {
                look_ahead();
                current_state = 90;
            }
            break;
        case 66:
            if (c == 'a')
                current_state = 67;
            else {
                look_ahead();
                current_state = 90;
            }
            break;
        case 67:
            if (c == 'r')
                current_state = 68;
            else {
                look_ahead();
                current_state = 90;
            }
            break;
        case 68:
            if (isValidIdChar(c))
                current_state = 90;
            else {
                look_ahead();
                token = Token(Token::Name::TYPE, Token::Type::CHAR, row, col_lex_init);
            }
            break;
        case 70:
            if (c == 'r')
                current_state = 71;
            else {
                look_ahead();
                current_state = 90;
            }
            break;
        case 71:
            if (c == 'o')
                current_state = 72;
            else {
                look_ahead();
                current_state = 90;
            }
            break;
        case 72:
            if (c == 'g')
                current_state = 73;
            else {
                look_ahead();
                current_state = 90;
            }
            break;
        case 73:
            if (c == 'r')
                current_state = 74;
            else {
                look_ahead();
                current_state = 90;
            }
            break;
        case 74:
            if (c == 'a')
                current_state = 75;
            else {
                look_ahead();
                current_state = 90;
            }
            break;
        case 75:
            if (c == 'm')
                current_state = 76;
            else {
                look_ahead();
                current_state = 90;
            }
            break;
        case 76:
            if (c == 'a')
                current_state = 77;
            else {
                look_ahead();
                current_state = 90;
            }
            break;
        case 77:
            if (isValidIdChar(c))
                current_state = 90;
            else {
                look_ahead();
                token = Token(Token::Name::PROGRAMA, nullptr, row, col_lex_init);
            }
            break;
        case 79:
            if (c == 'h')
                current_state = 80;
            else {
                look_ahead();
                current_state = 90;
            }
            break;
        case 80:
            if (c == 'e')
                current_state = 81;
            else {
                look_ahead();
                current_state = 90;
            }
            break;
        case 81:
            if (c == 'n')
                current_state = 82;
            else {
                look_ahead();
                current_state = 90;
            }
            break;
        case 82:
            if (isValidIdChar(c))
                current_state = 90;
            else {
                look_ahead();
                token = Token(Token::Name::THEN, nullptr, row, col_lex_init);
            }
            break;
        case 84:
            if (c == 'h')
                current_state = 85;
            else {
                look_ahead();
                current_state = 90;
            }
            break;
        case 85:
            if (c == 'i')
                current_state = 86;
            else {
                look_ahead();
                current_state = 90;
            }
            break;
        case 86:
            if (c == 'l')
                current_state = 87;
            else {
                look_ahead();
                current_state = 90;
            }
            break;
        case 87:
            if (c == 'e')
                current_state = 88;
            else {
                look_ahead();
                current_state = 90;
            }
            break;
        case 88:
            if (isValidIdChar(c))
                current_state = 90;
            else {
                look_ahead();
                token = Token(Token::Name::WHILE, nullptr, row, col_lex_init);
            }
            break;
        case 90:
            current_state = s90_id_tail(c);
            break;

        default:
            throw LexerException("Estado nao implementado: " + std::to_string(current_state), row, col, c);
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
        lexeme.pop_back();
        return 0;
    }
    col_lex_init = col;

    // TODO Este é o primeiro estado do diagrama, após descartar os caracteres white space, fazer a transição
    // para todos outros estados fingindo que estamos lendo o primeiro caractere.

    switch (c) {
    case ':':
        return 3;
    case ';':
        token = Token(Token::Name::END_SENTENCE, nullptr, row, col_lex_init);
        return -1;
    case '%': {
        return 6;
    }
    case '{':
        return 8;
    case ']':
        token = Token(Token::Name::BRACKET_END, nullptr, row, col);
        return -1;
    case '[':
        token = Token(Token::Name::BRACKET_START, nullptr, row, col);
        return -1;
    case '\'':
        return 15;
    case ',':
        token = Token(Token::Name::COMMA, nullptr, row, col);
        return -1;
    case '/':
        token = Token(Token::Name::OPMULDIV, Token::OpMulDiv::DIV, row, col);
        return -1;
    case '*':
        return 28;
    case '+':
        token = Token(Token::Name::OPSOMASUB, Token::OpSomaSub::SOMA, row, col);
        return -1;
    case '-':
        token = Token(Token::Name::OPSOMASUB, Token::OpSomaSub::SUB, row, col);
        return -1;
    case ')':
        token = Token(Token::Name::PAR_END, nullptr, row, col);
        return -1;
    case '(':
        token = Token(Token::Name::PAR_START, nullptr, row, col);
        return -1;
    case '<':
        return 35;
    case '>':
        return 39;
    case '=':
        token = Token(Token::Name::RELOP, Token::RelOp::EQ, row, col);
        return -1;
    case 'd':
        return 42;
    case 'e':
        return 45;
    case 'i':
        return 53;
    case 'f':
        return 59;
    case 'c':
        return 65;
    case 'p':
        return 70;
    case 't':
        return 79;
    case 'w':
        return 84;
    default:
        if (c >= '0' && c <= '9')
            return 20;
        if ((c >= 'A' && c <= 'Z') || std::string("abghjklmnoqrsuvxyz_").find(c) != std::string::npos)
            return 90;
    }

    throw LexerException("Transicao ainda nao implementada", row, col, c);
};

int Lexer::s3_colon(char c) {
    if (c == '=')
        return 5;
    else {
        look_ahead();
        token = Token(Token::Name::COLON, nullptr, row, col_lex_init);
        return -1;
    }
}

int Lexer::s20_num(char c) {
    if (c >= '0' && c <= '9')
        return 20;
    else if (c == '.')
        return 21;
    else if (c == 'E')
        return 23;
    else {
        return s26_num_f(0);
    }
}

int Lexer::s26_num_f(char c) {
    look_ahead();
    token = Token(Token::Name::NUM, lexeme, row, col);
    symbolTable.insert(Row(token.value()));
    return -1;
}

int Lexer::s90_id_tail(char c) {
    if (isValidIdChar(c)) {
        return 90;
    } else {
        look_ahead();
        token = Token(Token::Name::ID, lexeme, row, col);
        symbolTable.insert(Row(token.value()));
        return -1;
    }
};