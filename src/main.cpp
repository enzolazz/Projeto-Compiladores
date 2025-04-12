#include "Lexer.hpp"
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

    Lexer lex(source);

    for (auto tok = lex.next_token(); tok.has_value(); tok = lex.next_token()) {
        switch (tok->name) {
        case Token::Name::ID:
        case Token::Name::CONST:
            cout << "Linha da tabela: "
                 << lex.symbolTable[std::any_cast<SymbolTable::size_type>(tok->attribute)].to_string();
            break;
        default:
            cout << "Token lido: " << tok.value().to_string();
            break;
        }
        cout << endl;
    }

    return EXIT_SUCCESS;
}
