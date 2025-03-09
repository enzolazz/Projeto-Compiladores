#include "Lexer.hpp"
#include "SymbolTable.hpp"
#include "Token.hpp"
#include <cstdlib>
#include <fstream>
#include <iostream>

int main(int argc, char *argv[]) {
    using namespace std;

    if (argc != 2) {
        cerr << "Informe o nome do arquivo a ser lido como primeiro argumento "
                "da linha de comando."
             << endl;
        return EXIT_FAILURE;
    }

    ifstream source(argv[1]);
    if (!source.is_open()) {
        cout << "Error opening file!\n";
        return EXIT_FAILURE;
    }

    SymbolTable symbolTable;
    Lexer lex(source, symbolTable);

    while (!lex.isEOF()) {
        Token tok = lex.next_token();
        cout << "Token lido: " << tok.to_string() << endl;
    }
}
