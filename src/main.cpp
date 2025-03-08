#include "Lexer.hpp"
#include "Token.hpp"
#include "exception/LexerException.hpp"
#include <cstdlib>
#include <fstream>
#include <iostream>

int main(int argc, char *argv[]) {
    using namespace std;

    ifstream source(argv[1]);
    if (!source.is_open()) {
        cout << "Error opening file!\n";
        return EXIT_FAILURE;
    }

    Lexer lex(source);

    while (true) {
        Token tok = lex.next_token();
        cout << "Token lido: " << tok.to_string() << endl;
    }
}
