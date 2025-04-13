#include "SyntacticAnalyzer.hpp"
#include "exception/SyntaticException.hpp"
#include <algorithm>
#include <fstream>
#include <iostream>
#include <stack>
#include <variant>

template <typename Tp, typename Sequence = std::deque<Tp>> struct print_stack : std::stack<Tp, Sequence> {
    void print() const {
        std::cout << "Stack: ";
        for (const auto &s : this->c) {
            if (const auto v = std::get_if<Token::Name>(&s)) {
                std::cout << Token::to_string(*v) << "; ";
            } else {
                std::cout << *std::get_if<int>(&s) + 2 << "; ";
            }
        }
        std::cout << std::endl;
    }
};

SyntacticAnalyzer::SyntacticAnalyzer(std::ifstream &source) : lexer(source) {
    for (auto &l : table)
        std::fill_n(l, TABLE_COLUMNS, -1);

    table[NT::PROGRAMA][T::PROGRAMA] = 1;
    table[NT::BLOCO][T::BLOCO_START] = 2;
    table[NT::DECLARACOES][T::IF] = 4;
    table[NT::DECLARACOES][T::WHILE] = 4;
    table[NT::DECLARACOES][T::DO] = 4;
    table[NT::DECLARACOES][T::ID] = 4;
    table[NT::DECLARACOES][T::TYPE] = 3;
    table[NT::DECLARACAO][T::TYPE] = 5;
    table[NT::IDS][T::ID] = 6;
    table[NT::LISTA_IDS][T::END_SENTENCE] = 8;
    table[NT::LISTA_IDS][T::COMMA] = 7;
    table[NT::COMANDOS][T::IF] = 9;
    table[NT::COMANDOS][T::WHILE] = 9;
    table[NT::COMANDOS][T::DO] = 9;
    table[NT::COMANDOS][T::ID] = 9;
    table[NT::COMANDOS_OPT][T::IF] = 10;
    table[NT::COMANDOS_OPT][T::WHILE] = 10;
    table[NT::COMANDOS_OPT][T::DO] = 10;
    table[NT::COMANDOS_OPT][T::BLOCO_END] = 11;
    table[NT::COMANDOS_OPT][T::ID] = 10;
    table[NT::COMANDO][T::IF] = 13;
    table[NT::COMANDO][T::WHILE] = 14;
    table[NT::COMANDO][T::DO] = 14;
    table[NT::COMANDO][T::ID] = 12;
    table[NT::CMD_ATRIBUICAO][T::ID] = 15;
    table[NT::CMD_SELECAO][T::IF] = 16;
    table[NT::CMD_ELSE_OPT][T::IF] = 19;
    table[NT::CMD_ELSE_OPT][T::ELSE] = 18;
    table[NT::CMD_ELSE_OPT][T::ELSEIF] = 17;
    table[NT::CMD_ELSE_OPT][T::WHILE] = 19;
    table[NT::CMD_ELSE_OPT][T::DO] = 19;
    table[NT::CMD_ELSE_OPT][T::BLOCO_END] = 19;
    table[NT::CMD_ELSE_OPT][T::ID] = 19;
    table[NT::CMD_REPETICAO][T::WHILE] = 20;
    table[NT::CMD_REPETICAO][T::DO] = 21;
    table[NT::CMD_OU_BLOCO][T::IF] = 22;
    table[NT::CMD_OU_BLOCO][T::WHILE] = 22;
    table[NT::CMD_OU_BLOCO][T::DO] = 22;
    table[NT::CMD_OU_BLOCO][T::BLOCO_START] = 23;
    table[NT::CMD_OU_BLOCO][T::ID] = 22;
    table[NT::CONDICAO][T::PAR_START] = 24;
    table[NT::CONDICAO][T::ID] = 24;
    table[NT::CONDICAO][T::CONST] = 24;
    table[NT::CONDICAO][T::SUM] = 24;
    table[NT::CONDICAO][T::SUB] = 24;
    table[NT::EXPRESSAO][T::PAR_START] = 25;
    table[NT::EXPRESSAO][T::ID] = 25;
    table[NT::EXPRESSAO][T::CONST] = 25;
    table[NT::EXPRESSAO][T::SUM] = 25;
    table[NT::EXPRESSAO][T::SUB] = 25;
    table[NT::EXPRESSAO_PRIME][T::END_SENTENCE] = 28;
    table[NT::EXPRESSAO_PRIME][T::PAR_END] = 28;
    table[NT::EXPRESSAO_PRIME][T::BRACKET_END] = 28;
    table[NT::EXPRESSAO_PRIME][T::RELOP] = 28;
    table[NT::EXPRESSAO_PRIME][T::SUM] = 26;
    table[NT::EXPRESSAO_PRIME][T::SUB] = 27;
    table[NT::TERMO][T::PAR_START] = 29;
    table[NT::TERMO][T::ID] = 29;
    table[NT::TERMO][T::CONST] = 29;
    table[NT::TERMO][T::SUM] = 29;
    table[NT::TERMO][T::SUB] = 29;
    table[NT::TERMO_PRIME][T::END_SENTENCE] = 32;
    table[NT::TERMO_PRIME][T::PAR_END] = 32;
    table[NT::TERMO_PRIME][T::BRACKET_END] = 32;
    table[NT::TERMO_PRIME][T::RELOP] = 32;
    table[NT::TERMO_PRIME][T::SUM] = 32;
    table[NT::TERMO_PRIME][T::SUB] = 32;
    table[NT::TERMO_PRIME][T::MUL] = 30;
    table[NT::TERMO_PRIME][T::DIV] = 31;
    table[NT::POTENCIA][T::PAR_START] = 33;
    table[NT::POTENCIA][T::ID] = 33;
    table[NT::POTENCIA][T::CONST] = 33;
    table[NT::POTENCIA][T::SUM] = 33;
    table[NT::POTENCIA][T::SUB] = 33;
    table[NT::POTENCIA_PRIME][T::END_SENTENCE] = 35;
    table[NT::POTENCIA_PRIME][T::PAR_END] = 35;
    table[NT::POTENCIA_PRIME][T::BRACKET_END] = 35;
    table[NT::POTENCIA_PRIME][T::RELOP] = 35;
    table[NT::POTENCIA_PRIME][T::SUM] = 35;
    table[NT::POTENCIA_PRIME][T::SUB] = 35;
    table[NT::POTENCIA_PRIME][T::MUL] = 35;
    table[NT::POTENCIA_PRIME][T::DIV] = 35;
    table[NT::POTENCIA_PRIME][T::POW] = 34;
    table[NT::FATOR][T::PAR_START] = 36;
    table[NT::FATOR][T::ID] = 37;
    table[NT::FATOR][T::CONST] = 38;
    table[NT::FATOR][T::SUM] = 39;
    table[NT::FATOR][T::SUB] = 40;
}

void SyntacticAnalyzer::literalmenteQualquerCoisa() {
    print_stack<std::variant<Token::Name, int>> stack;
    stack.push(NT::PROGRAMA);

    auto [nextToken, row, col] = lexer.next_token();

    while (!stack.empty()) {
        auto X = stack.top();

        if (auto v = std::get_if<Token::Name>(&X)) {
            stack.print();
            std::cout << '\n' << std::endl;
            if (*v == nextToken.name) {
                stack.pop();
                std::tie(nextToken, row, col) = lexer.next_token();
            } else {
                throw SyntacticException("Token esperado: " + Token::to_string(*v), nextToken, row, col);
            }
        } else {
            int nt = *std::get_if<int>(&X);
            int prod = table[nt][static_cast<int>(nextToken.name)];
            stack.print();
            std::cout << "Producao: " << prod << '\n' << std::endl;
            if (prod == -1)
                throw SyntacticException("Producao inexistente.", nextToken, row, col);

            // figurativamenteQualquerCoisa(a);
            stack.pop();
            switch (prod) {
            case 1:
                stack.emplace(NT::BLOCO);
                stack.emplace(Token::Name::PAR_END);
                stack.emplace(Token::Name::PAR_START);
                stack.emplace(Token::Name::ID);
                stack.emplace(Token::Name::PROGRAMA);
                break;
            case 2:
                stack.emplace(Token::Name::BLOCO_END);
                stack.emplace(NT::COMANDOS);
                stack.emplace(NT::DECLARACOES);
                stack.emplace(Token::Name::BLOCO_START);
                break;
            case 3:
                stack.emplace(NT::DECLARACOES);
                stack.emplace(NT::DECLARACAO);
                break;
            case 5:
                stack.emplace(Token::Name::END_SENTENCE);
                stack.emplace(NT::IDS);
                stack.emplace(Token::Name::COLON);
                stack.emplace(Token::Name::TYPE);
                break;
            case 6:
                stack.emplace(NT::LISTA_IDS);
                stack.emplace(Token::Name::ID);
                break;
            case 7:
                stack.emplace(NT::LISTA_IDS);
                stack.emplace(Token::Name::ID);
                stack.emplace(Token::Name::COMMA);
                break;
            case 9:
                stack.emplace(NT::COMANDOS_OPT);
                stack.emplace(NT::COMANDO);
                break;
            case 10:
                stack.emplace(NT::COMANDOS);
                break;
            case 12:
                stack.emplace(NT::CMD_ATRIBUICAO);
                break;
            case 13:
                stack.emplace(NT::CMD_SELECAO);
                break;
            case 14:
                stack.emplace(NT::CMD_REPETICAO);
                break;
            case 15:
                stack.emplace(Token::Name::END_SENTENCE);
                stack.emplace(NT::EXPRESSAO);
                stack.emplace(Token::Name::ATTRIBUTION);
                stack.emplace(Token::Name::ID);
                break;
            case 16:
                stack.emplace(NT::CMD_ELSE_OPT);
                stack.emplace(NT::CMD_OU_BLOCO);
                stack.emplace(Token::Name::THEN);
                stack.emplace(Token::Name::BRACKET_END);
                stack.emplace(NT::CONDICAO);
                stack.emplace(Token::Name::BRACKET_START);
                stack.emplace(Token::Name::IF);
                break;
            case 17:
                stack.emplace(NT::CMD_ELSE_OPT);
                stack.emplace(NT::CMD_OU_BLOCO);
                stack.emplace(Token::Name::THEN);
                stack.emplace(Token::Name::BRACKET_END);
                stack.emplace(NT::CONDICAO);
                stack.emplace(Token::Name::BRACKET_START);
                stack.emplace(Token::Name::ELSEIF);
                break;
            case 18:
                stack.emplace(NT::CMD_OU_BLOCO);
                stack.emplace(Token::Name::ELSE);
                break;
            case 20:
                stack.emplace(NT::CMD_OU_BLOCO);
                stack.emplace(Token::Name::DO);
                stack.emplace(Token::Name::BRACKET_END);
                stack.emplace(NT::CONDICAO);
                stack.emplace(Token::Name::BRACKET_START);
                stack.emplace(Token::Name::WHILE);
                break;
            case 21:
                stack.emplace(Token::Name::END_SENTENCE);
                stack.emplace(Token::Name::BRACKET_END);
                stack.emplace(NT::CONDICAO);
                stack.emplace(Token::Name::BRACKET_START);
                stack.emplace(Token::Name::WHILE);
                stack.emplace(NT::CMD_OU_BLOCO);
                stack.emplace(Token::Name::DO);
                break;
            case 22:
                stack.emplace(NT::COMANDO);
                break;
            case 23:
                stack.emplace(NT::BLOCO);
                break;
            case 24:
                stack.emplace(NT::EXPRESSAO);
                stack.emplace(Token::Name::RELOP);
                stack.emplace(NT::EXPRESSAO);
                break;
            case 25:
                stack.emplace(NT::EXPRESSAO_PRIME);
                stack.emplace(NT::TERMO);
                break;
            case 26:
                stack.emplace(NT::EXPRESSAO_PRIME);
                stack.emplace(NT::TERMO);
                stack.emplace(Token::Name::SUM);
                break;
            case 27:
                stack.emplace(NT::EXPRESSAO_PRIME);
                stack.emplace(NT::TERMO);
                stack.emplace(Token::Name::SUB);
                break;
            case 29:
                stack.emplace(NT::TERMO_PRIME);
                stack.emplace(NT::POTENCIA);
                break;
            case 30:
                stack.emplace(NT::TERMO_PRIME);
                stack.emplace(NT::POTENCIA);
                stack.emplace(Token::Name::MUL);
                break;
            case 31:
                stack.emplace(NT::TERMO_PRIME);
                stack.emplace(NT::POTENCIA);
                stack.emplace(Token::Name::DIV);
                break;
            case 33:
                stack.emplace(NT::POTENCIA_PRIME);
                stack.emplace(NT::FATOR);
                break;
            case 34:
                stack.emplace(NT::POTENCIA_PRIME);
                stack.emplace(NT::FATOR);
                stack.emplace(Token::Name::POW);
                break;
            case 36:
                stack.emplace(Token::Name::PAR_END);
                stack.emplace(NT::EXPRESSAO);
                stack.emplace(Token::Name::PAR_START);
                break;
            case 37:
                stack.emplace(Token::Name::ID);
                break;
            case 38:
                stack.emplace(Token::Name::CONST);
                break;
            case 39:
                stack.emplace(NT::FATOR);
                stack.emplace(Token::Name::SUM);
                break;
            case 40:
                stack.emplace(NT::FATOR);
                stack.emplace(Token::Name::SUB);
                break;
            default:
                if (prod > 40)
                    throw std::logic_error("Producao inexistente.");
                break;
            }
        }
    }
    if (nextToken.name != Token::Name::END_OF_FILE)
        throw SyntacticException("O fim do arquivo era esperado.", nextToken, row, col);
}
