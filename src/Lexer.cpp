#include "Lexer.hpp"
#include "SymbolTable.hpp"
#include "Token.hpp"
#include "exception/LexerException.hpp"
#include <cctype>
#include <fstream>
#include <stdexcept>
#include <string>

constexpr static signed char eof_c = -1;

Lexer::Lexer(std::ifstream &source, SymbolTable &symbolTable)
    : source(source), symbolTable(symbolTable), active_buffer(0), row(1), col(1), col_lex_init(1), next_pos(0),
      eof(false) {
    source.read(buffers[0], BUFFER_SIZE);
    if (source.gcount() != BUFFER_SIZE)
        buffers[active_buffer][source.gcount()] = eof_c;
    else {
        source.read(buffers[1], BUFFER_SIZE);
        if (source.gcount() != BUFFER_SIZE)
            buffers[active_buffer][source.gcount()] = eof_c;
    }
}

signed char Lexer::next_char() {
    signed char ret;
    if (next_pos == BUFFER_SIZE) {
        source.read(buffers[active_buffer], BUFFER_SIZE);
        if (source.gcount() != BUFFER_SIZE)
            buffers[active_buffer][source.gcount()] = eof_c;
        active_buffer ^= 1;
        ret = buffers[active_buffer][0];
        next_pos = 1;
    } else {
        ret = buffers[active_buffer][next_pos];
        ++next_pos;
    }

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

static bool isValidIdChar(signed char c) { return std::isalnum(c) || c == '_'; }

static signed char to_char(const std::string &lexeme) {
    if (lexeme[1] == '\\') {
        switch (lexeme[2]) {
        case 'a':
            return '\a';
        case 'b':
            return '\b';
        case 't':
            return '\t';
        case 'n':
            return '\n';
        case 'v':
            return '\v';
        case 'f':
            return '\f';
        case 'r':
            return '\r';
        default:
            return lexeme[2];
        }
    }
    return lexeme[1];
}

// optional.has_value() == false;
std::optional<Token> Lexer::next_token() {
    int current_state = 0;
    token = {};
    lexeme = {};
    signed char c;
    const auto nc = [&c, this]() {
        c = next_char();
        this->lexeme += c;
    };

    while (!token.has_value()) {
        if (eof)
            return {};

        switch (current_state) {
        case 0:
            nc();
            if (std::isspace(c)) {
                lexeme.pop_back();
                current_state = 1;
            } else
                current_state = s0_inicio_token(c);
            break;
        case 1:
            nc();
            if (std::isspace(c)) {
                lexeme.pop_back();
                current_state = 1;
            } else
                current_state = 2;
            break;
        case 2:
            look_ahead();
            current_state = 0;
            break;
        case 3:
            nc();
            if (c == '=')
                current_state = 5;
            else
                current_state = 4;
            break;
        case 4:
            look_ahead();
            token = Token(Token::Name::COLON, nullptr, row, col_lex_init);
            current_state = -1;
            break;
        case 5:
            nc();
            token = Token(Token::Name::ATTRIBUTION, nullptr, row, col_lex_init);
            break;
        case 6:
            nc();
            if (c == '}')
                current_state = 7;
            else
                throw LexerException("Caractere não reconhecido", row, col, c);
            break;
        case 7:
            token = Token(Token::Name::BLOCO_END, nullptr, row, col_lex_init);
            break;
        case 8:
            nc();
            if (c == '%')
                current_state = 9;
            else if (c == '#')
                current_state = 10;
            else
                throw LexerException("Caractere não reconhecido", row, col, c);
            break;
        case 9:
            token = Token(Token::Name::BLOCO_START, nullptr, row, col_lex_init);
            break;
        case 10:
            nc();
            if (c == '#')
                current_state = 11;
            else if (c == eof_c)
                throw LexerException("Fim inesperado do arquivo", row, col, eof_c);
            else
                current_state = 10;
            break;
        case 11:
            nc();
            if (c == '}')
                current_state = 12;
            break;
        case 12:
            current_state = 0;
            lexeme = {};
            break;
        case 13:
            token = Token(Token::Name::BRACKET_END, nullptr, row, col);
            current_state = -1;
            break;
        case 14:
            token = Token(Token::Name::BRACKET_START, nullptr, row, col);
            current_state = -1;
            break;
        case 15:
            nc();
            if (c == eof_c)
                throw LexerException("Fim inesperado do arquivo", row, col, c);
            else if (c == '\\')
                current_state = 16;
            else if (c != '\\' && c != '\'' && c != '\n')
                current_state = 17;
            else
                throw LexerException("Caractere inesperado", row, col, c);
            break;
        case 16:
            nc();
            if (c != '\n' && c != '\t')
                current_state = 17;
            else
                throw LexerException("Caractere inesperado", row, col, c);
            break;
        case 17:
            nc();
            if (c == '\'')
                current_state = 93;
            else
                throw LexerException("Caractere inesperado", row, col, c);
            break;
        case 18:
            token = Token(Token::Name::COMMA, nullptr, row, col);
            current_state = -1;
            break;
        case 19:
            token = Token(Token::Name::END_SENTENCE, nullptr, row, col_lex_init);
            current_state = -1;
            break;
        case 20:
            nc();
            current_state = s20_num(c);
            break;
        case 21:
            nc();
            if (c >= '0' && c <= '9')
                current_state = 22;
            else
                throw LexerException("Caractere não reconhecido", row, col, c);
            break;
        case 22:
            nc();
            if (c >= '0' && c <= '9')
                current_state = 22;
            else if (c == 'E')
                current_state = 23;
            else
                current_state = 26;
            break;
        case 23:
            nc();
            if (c == '+' || c == '-')
                current_state = 24;
            else if (c >= '0' && c <= '9')
                current_state = 25;
            else
                throw LexerException("Caractere não reconhecido", row, col, c);
            break;
        case 24:
            nc();
            if (c >= '0' && c <= '9')
                current_state = 25;
            else
                throw LexerException("Caractere não reconhecido", row, col, c);
            break;
        case 25:
            nc();
            if (c >= '0' && c <= '9')
                current_state = 25;
            else
                current_state = 26;
            break;
        case 26: {
            look_ahead();
            token = Token(Token::Name::CONST, nullptr, row, col);
            auto index = symbolTable.insert(Row(token.value(), lexeme));
            symbolTable[index].token.attribute = index;
            token = symbolTable[index].token;
            current_state = -1;
            break;
        }
        case 27:
            token = Token(Token::Name::DIV, nullptr, row, col);
            current_state = -1;
            break;
        case 28:
            nc();
            if (c == '*')
                current_state = 30;
            else
                current_state = 29;
            break;
        case 29:
            look_ahead();
            token = Token(Token::Name::MUL, nullptr, row, col_lex_init);
            current_state = -1;
            break;
        case 30:
            token = Token(Token::Name::POW, nullptr, row, col_lex_init);
            current_state = -1;
            break;
        case 31:
            token = Token(Token::Name::SUM, nullptr, row, col);
            current_state = -1;
            break;
        case 32:
            token = Token(Token::Name::SUB, nullptr, row, col);
            current_state = -1;
            break;
        case 33:
            token = Token(Token::Name::PAR_END, nullptr, row, col);
            current_state = -1;
            break;
        case 34:
            token = Token(Token::Name::PAR_START, nullptr, row, col);
            current_state = -1;
            break;
        case 35:
            nc();
            switch (c) {
            case '=':
                current_state = 38;
            case '>':
                current_state = 36;
            default:
                current_state = 37;
            }
            break;
        case 36:
            token = Token(Token::Name::RELOP, Token::RelOp::NE, row, col_lex_init);
            current_state = -1;
            break;
        case 37:
            look_ahead();
            token = Token(Token::Name::RELOP, Token::RelOp::LT, row, col_lex_init);
            current_state = -1;
            break;
        case 38:
            token = Token(Token::Name::RELOP, Token::RelOp::LE, row, col_lex_init);
            current_state = -1;
            break;
        case 39:
            nc();
            if (c == '=')
                current_state = 41;
            else
                current_state = 40;
            break;
        case 40:
            look_ahead();
            token = Token(Token::Name::RELOP, Token::RelOp::GT, row, col_lex_init);
            break;
        case 41:
            token = Token(Token::Name::RELOP, Token::RelOp::GE, row, col_lex_init);
            current_state = -1;
            break;
        case 42:
            nc();
            if (c == 'o')
                current_state = 43;
            else
                current_state = s90_id_tail(c);
            break;
        case 43:
            nc();
            if (isValidIdChar(c))
                current_state = 90;
            else
                current_state = 44;
            break;
        case 44:
            look_ahead();
            token = Token(Token::Name::DO, nullptr, row, col_lex_init);
            current_state = -1;
            break;
        case 45:
            nc();
            if (c == 'l')
                current_state = 46;
            else
                current_state = s90_id_tail(c);
            break;
        case 46:
            nc();
            if (c == 's')
                current_state = 47;
            else
                current_state = s90_id_tail(c);
            break;
        case 47:
            nc();
            if (c == 'e')
                current_state = 48;
            else
                current_state = s90_id_tail(c);
            break;
        case 48:
            nc();
            if (c == 'i')
                current_state = 50;
            else
                current_state = s90_id_tail(c);
            break;
        case 49:
            look_ahead();
            token = Token(Token::Name::ELSE, nullptr, row, col_lex_init);
            current_state = -1;
            break;
        case 50:
            nc();
            if (c == 'f')
                current_state = 51;
            else
                current_state = s90_id_tail(c);
            break;
        case 51:
            nc();
            if (isValidIdChar(c))
                current_state = 90;
            else
                current_state = 52;
            break;
        case 52:
            look_ahead();
            token = Token(Token::Name::ELSEIF, nullptr, row, col_lex_init);
            current_state = -1;
            break;
        case 53:
            nc();
            if (c == 'f')
                current_state = 54;
            else if (c == 'n')
                current_state = 56;
            else
                current_state = s90_id_tail(c);
            break;
        case 54:
            nc();
            if (isValidIdChar(c))
                current_state = 90;
            else
                current_state = 55;
            break;
        case 55:
            look_ahead();
            token = Token(Token::Name::IF, nullptr, row, col_lex_init);
            current_state = -1;
            break;
        case 56:
            nc();
            if (c == 't')
                current_state = 57;
            else
                current_state = s90_id_tail(c);
            break;
        case 57:
            nc();
            if (isValidIdChar(c))
                current_state = 90;
            else
                current_state = 58;
            break;
        case 58:
            look_ahead();
            token = Token(Token::Name::TYPE, Token::Type::INT, row, col_lex_init);
            break;
        case 59:
            nc();
            if (c == 'l')
                current_state = 60;
            else
                current_state = s90_id_tail(c);
            break;
        case 60:
            nc();
            if (c == 'o')
                current_state = 61;
            else
                current_state = s90_id_tail(c);
            break;
        case 61:
            nc();
            if (c == 'a')
                current_state = 62;
            else
                current_state = s90_id_tail(c);
            break;
        case 62:
            nc();
            if (c == 't')
                current_state = 63;
            else
                current_state = s90_id_tail(c);
            break;
        case 63:
            nc();
            if (isValidIdChar(c))
                current_state = 90;
            else
                current_state = 64;
            break;
        case 64:
            look_ahead();
            token = Token(Token::Name::TYPE, Token::Type::FLOAT, row, col_lex_init);
            current_state = -1;
            break;
        case 65:
            nc();
            if (c == 'h')
                current_state = 66;
            else
                current_state = s90_id_tail(c);
            break;
        case 66:
            nc();
            if (c == 'a')
                current_state = 67;
            else
                current_state = s90_id_tail(c);
            break;
        case 67:
            nc();
            if (c == 'r')
                current_state = 68;
            else
                current_state = s90_id_tail(c);
            break;
        case 68:
            nc();
            if (isValidIdChar(c))
                current_state = 90;
            else
                current_state = 69;
            break;
        case 69:
            look_ahead();
            token = Token(Token::Name::TYPE, Token::Type::CHAR, row, col_lex_init);
            current_state = -1;
            break;
        case 70:
            nc();
            if (c == 'r')
                current_state = 71;
            else
                current_state = s90_id_tail(c);
            break;
        case 71:
            nc();
            if (c == 'o')
                current_state = 72;
            else
                current_state = s90_id_tail(c);
            break;
        case 72:
            nc();
            if (c == 'g')
                current_state = 73;
            else
                current_state = s90_id_tail(c);
            break;
        case 73:
            nc();
            if (c == 'r')
                current_state = 74;
            else
                current_state = s90_id_tail(c);
            break;
        case 74:
            nc();
            if (c == 'a')
                current_state = 75;
            else
                current_state = s90_id_tail(c);
            break;
        case 75:
            nc();
            if (c == 'm')
                current_state = 76;
            else
                current_state = s90_id_tail(c);
            break;
        case 76:
            nc();
            if (c == 'a')
                current_state = 77;
            else
                current_state = s90_id_tail(c);
            break;
        case 77:
            nc();
            if (isValidIdChar(c))
                current_state = 90;
            else
                current_state = 78;
            break;
        case 78:
            look_ahead();
            token = Token(Token::Name::PROGRAMA, nullptr, row, col_lex_init);
            current_state = -1;
            break;
        case 79:
            nc();
            if (c == 'h')
                current_state = 80;
            else
                current_state = s90_id_tail(c);
            break;
        case 80:
            nc();
            if (c == 'e')
                current_state = 81;
            else
                current_state = s90_id_tail(c);
            break;
        case 81:
            nc();
            if (c == 'n')
                current_state = 82;
            else
                current_state = s90_id_tail(c);
            break;
        case 82:
            nc();
            if (isValidIdChar(c))
                current_state = 90;
            else
                current_state = 83;
            break;
        case 83:
            look_ahead();
            token = Token(Token::Name::THEN, nullptr, row, col_lex_init);
            current_state = -1;
            break;
        case 84:
            nc();
            if (c == 'h')
                current_state = 85;
            else
                current_state = s90_id_tail(c);
            break;
        case 85:
            nc();
            if (c == 'i')
                current_state = 86;
            else
                current_state = s90_id_tail(c);
            break;
        case 86:
            nc();
            if (c == 'l')
                current_state = 87;
            else
                current_state = s90_id_tail(c);
            break;
        case 87:
            nc();
            if (c == 'e')
                current_state = 88;
            else
                current_state = s90_id_tail(c);
            break;
        case 88:
            nc();
            if (isValidIdChar(c))
                current_state = 90;
            else
                current_state = 89;
            break;
        case 89:
            look_ahead();
            token = Token(Token::Name::WHILE, nullptr, row, col_lex_init);
            current_state = -1;
            break;
        case 90:
            nc();
            current_state = s90_id_tail(c);
            break;
        case 91: {
            look_ahead();
            token = Token(Token::Name::ID, nullptr, row, col);
            auto index = symbolTable.insert(Row(token.value(), lexeme));
            symbolTable[index].token.attribute = index;
            token = symbolTable[index].token;
            current_state = -1;
            break;
        }
        case 92:
            token = Token(Token::Name::RELOP, Token::RelOp::EQ, row, col);
            current_state = -1;
            break;
        case 93: {
            token = Token(Token::Name::CONST, nullptr, row, col_lex_init);
            auto index = symbolTable.insert(Row(token.value(), lexeme));
            symbolTable[index].token.attribute = index;
            token = symbolTable[index].token;
            break;
        }
        default:
            throw LexerException("Estado nao implementado: " + std::to_string(current_state), row, col, c);
        }

        col++;
        if (c == '\n') {
            row++;
            col = 1;
        }

        if (c == eof_c)
            eof = true;
    }

    return token;
}

int Lexer::s0_inicio_token(signed char c) {
    if (c == eof_c) {
        eof = true;
        return 0;
    }
    col_lex_init = col;

    switch (c) {
    case ':':
        return 3;
    case ';':
        return 19;
    case '%':
        return 6;
    case '{':
        return 8;
    case ']':
        return 13;
    case '[':
        return 14;
    case '\'':
        return 15;
    case ',':
        return 18;
    case '/':
        return 27;
    case '*':
        return 28;
    case '+':
        return 31;
    case '-':
        return 32;
    case ')':
        return 33;
    case '(':
        return 34;
    case '<':
        return 35;
    case '>':
        return 39;
    case '=':
        return 92;
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

int Lexer::s20_num(signed char c) {
    if (c >= '0' && c <= '9')
        return 20;
    else if (c == '.')
        return 21;
    else if (c == 'E')
        return 23;
    else
        return 26;
}

int Lexer::s90_id_tail(signed char c) {
    if (isValidIdChar(c)) {
        return 90;
    } else {
        return 91;
    }
}
