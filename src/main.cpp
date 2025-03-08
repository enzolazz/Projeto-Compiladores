#include "Lex.hpp"
#include "Token.hpp"
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

    Lex lex(source);

    while (true) {
        try {
            Token tok = lex.next_token();
            cout << "Token lido: " << tok.to_string() << endl;
        } catch (int err) {
            cerr << "Not a valid token\n"
                 << "Error: " << err << endl;
            return EXIT_FAILURE;
        }
    }
}
