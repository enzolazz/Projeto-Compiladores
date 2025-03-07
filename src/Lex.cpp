#include "Lex.hpp"
#include "Token.hpp"
#include "include/Lex.hpp"
#include <cctype>
#include <iostream>

Lex::Lex() = default;

Lex::Lex(FILE *source) : source(source) {
    row = 1;
    col = 1;
    pos = BUFFER_SIZE;
}

char Lex::next_char() {
    if (pos == BUFFER_SIZE) {
        fgets(buffer1, BUFFER_SIZE, source);
        pos = 0;
    }

    return buffer1[pos++];
}

Token Lex::next_token() {
    std::string lex = "";
    bool found = false;

    int state = 0;
    while (!found) {
        lex += c;
        switch (state) {
        case 0:
            c = next_char();
            state = (c == 'w') ? 1 : -1;
            break;
        case 1:
            c = next_char();
            state = (c == 'h') ? 2 : -1;
            break;
        case 2:
            c = next_char();
            state = (c == 'i') ? 3 : -1;
            break;
        case 3:
            c = next_char();
            state = (c == 'l') ? 4 : -1;
            break;
        case 4:
            c = next_char();
            state = (c == 'e') ? 5 : -1;
            break;
        case 5:
            c = next_char();
            if (!isalnum(c) && c != '_')
                state = 6;
            else
                state = -1;
            break;
        case 6:
            lookAhead();
            return Token(TokenName::WHILE, nullptr, row, col);
        case -1:
            c = next_char();
            if (c == ' ' || c == '\0')
                col++;
            else if (c == '\t')
                col += 4;
            else if (c == '\n') {
                col = 0;
                row++;
            } else {
                lookAhead();
                std::cout << c << " at: (" << row << ", " << col << ")\n";
                throw 505;
            }
            break;
        default:
            break;
        }

        std::cout << c << ", " << state << std::endl;
        col++;
    }
}

void Lex::lookAhead() {
    col--;
    pos -= 2;
    c = next_char();
}
