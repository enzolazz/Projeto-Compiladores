#include <SyntacticAnalyzer.hpp>
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

    SyntacticAnalyzer syntactic_analyzer(source);
    cout << syntactic_analyzer.get_concrete_derivation_tree().to_string() << endl;

    return EXIT_SUCCESS;
}
