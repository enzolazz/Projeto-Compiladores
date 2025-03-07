#include "Lex.hpp"
#include "Token.hpp"
#include <iostream>

int main() {
    using namespace std;

    FILE *source = fopen("src/teste.txt", "r");
    if (source == nullptr) {
        cout << "Error opening file!\n";
        return -1;
    }

    Lex lex = Lex(source);
    Token tok;
    while (true) {
        try {
            tok = lex.next_token();
        } catch (int err) {
            cout << "Not a valid token\n";
            cout << "Error: " << err << endl;
            break;
        }

        cout << static_cast<int>(tok.name) << endl;
    }
}
