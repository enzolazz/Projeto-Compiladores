#include "Lexer.hpp"
#include "SymbolTable.hpp"
#include "Token.hpp"
#include <any>
#include <cstdlib>
#include <fstream>
#include <functional>
#include <iostream>
#include <ostream>
#include <string>

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

    for (auto tok = lex.next_token(); tok.has_value(); tok = lex.next_token()) {
        cout << "Token lido: " << tok.value().to_string();
        switch (tok->name) {
        case Token::Name::ID:
        case Token::Name::NUM:
            cout << " Linha da tabela: "
                 << symbolTable[std::any_cast<SymbolTable::size_type>(tok->attribute)].to_string();
            break;
        case Token::Name::CARACTERE:
            cout << " Linha da tabela: "
                 << symbolTable[std::any_cast<SymbolTable::size_type>(tok->attribute)].to_string();
            break;
        default:
            break;
        }
        cout << endl;
    }
}
