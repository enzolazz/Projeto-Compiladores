#include "SyntacticAnalyzer.hpp"
#include <algorithm>
#include <fstream>
#include <iostream>
#include <ranges>
#include <stack>
#include <variant>

template <typename Tp, typename Sequence = std::deque<Tp>> struct print_stack : std::stack<Tp, Sequence> {
    void print() const {
        std::cout << "Stack: ";
        for (const auto &s : this->c) {
            if (const auto name = std::get_if<Token::Name>(&s))
                std::cout << Token::to_string(*name) << "; ";
            else if (const auto nt = std::get_if<int>(&s))
                std::cout << *nt + 2 << "; ";
            else if (const auto e = std::get_if<ArvoreConcreta::epsilon_type>(&s))
                std::cout << "epsilon; ";
        }
        std::cout << std::endl;
    }
};

SyntacticAnalyzer::SyntacticAnalyzer(std::ifstream &source) : lexer(source), arvore(NT::PROGRAMA) {
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
    using clist = std::initializer_list<ArvoreConcreta::elem_type>;
    print_stack<ArvoreConcreta::elem_type> stack;
    stack.push(NT::PROGRAMA);

    auto [nextToken, row, col] = lexer.next_token();

    while (!stack.empty()) {
        auto X = stack.top();

        if (auto v = std::get_if<Token::Name>(&X)) {
            stack.print();
            std::cout << '\n' << std::endl;
            if (*v == nextToken.name) {
                stack.pop();
                arvore.next_node();
                std::tie(nextToken, row, col) = lexer.next_token();
            } else {
                std::cerr << Token::to_string(*v) << " != " << Token::to_string(nextToken.name) << std::endl;
                throw 0;
            }
        } else {
            int nt = *std::get_if<int>(&X);
            int prod = table[nt][static_cast<int>(nextToken.name)];
            stack.print();
            std::cout << "Producao: " << prod << '\n' << std::endl;
            if (prod == -1)
                throw -1;

            stack.pop();
            clist children;
            switch (prod) {
            case 1: {
                children = {NT::BLOCO, Token::Name::PAR_END, Token::Name::PAR_START, Token::Name::ID,
                            Token::Name::PROGRAMA};
                break;
            }
            case 2: {
                children = {Token::Name::BLOCO_END, NT::COMANDOS, NT::DECLARACOES, Token::Name::BLOCO_START};
                break;
            }
            case 3: {
                children = {NT::DECLARACOES, NT::DECLARACAO};
                break;
            }
            case 5: {
                children = {Token::Name::END_SENTENCE, NT::IDS, Token::Name::COLON, Token::Name::TYPE};
                break;
            }
            case 6: {
                children = {NT::LISTA_IDS, Token::Name::ID};
                break;
            }
            case 7: {
                children = {NT::LISTA_IDS, Token::Name::ID, Token::Name::COMMA};
                break;
            }
            case 9: {
                children = {NT::COMANDOS_OPT, NT::COMANDO};
                break;
            }
            case 10:
                children = {NT::COMANDOS};
                break;
            case 12:
                children = {NT::CMD_ATRIBUICAO};
                break;
            case 13:
                children = {NT::CMD_SELECAO};
                break;
            case 14:
                children = {NT::CMD_REPETICAO};
                break;
            case 15:
                children = {Token::Name::END_SENTENCE, NT::EXPRESSAO, Token::Name::ATTRIBUTION, Token::Name::ID};
                break;
            case 16:
                children = {NT::CMD_ELSE_OPT, NT::CMD_OU_BLOCO,           Token::Name::THEN, Token::Name::BRACKET_END,
                            NT::CONDICAO,     Token::Name::BRACKET_START, Token::Name::IF};
                break;
            case 17:
                children = {NT::CMD_ELSE_OPT, NT::CMD_OU_BLOCO,           Token::Name::THEN,  Token::Name::BRACKET_END,
                            NT::CONDICAO,     Token::Name::BRACKET_START, Token::Name::ELSEIF};
                break;
            case 18:
                children = {NT::CMD_OU_BLOCO, Token::Name::ELSE};
                break;
            case 20:
                children = {NT::CMD_OU_BLOCO,           Token::Name::DO,   Token::Name::BRACKET_END, NT::CONDICAO,
                            Token::Name::BRACKET_START, Token::Name::WHILE};
                break;
            case 21:
                children = {
                    Token::Name::END_SENTENCE, Token::Name::BRACKET_END, NT::CONDICAO,   Token::Name::BRACKET_START,
                    Token::Name::WHILE,        NT::CMD_OU_BLOCO,         Token::Name::DO};
                break;
            case 22:
                children = {NT::COMANDO};
                break;
            case 23:
                children = {NT::BLOCO};
                break;
            case 24:
                children = {NT::EXPRESSAO, Token::Name::RELOP, NT::EXPRESSAO};
                break;
            case 25:
                children = {NT::EXPRESSAO_PRIME, NT::TERMO};
                break;
            case 26:
                children = {NT::EXPRESSAO_PRIME, NT::TERMO, Token::Name::SUM};
                break;
            case 27:
                children = {NT::EXPRESSAO_PRIME, NT::TERMO, Token::Name::SUB};
                break;
            case 29:
                children = {NT::TERMO_PRIME, NT::POTENCIA};
                break;
            case 30:
                children = {NT::TERMO_PRIME, NT::POTENCIA, Token::Name::MUL};
                break;
            case 31:
                children = {NT::TERMO_PRIME, NT::POTENCIA, Token::Name::DIV};
                break;
            case 33:
                children = {NT::POTENCIA_PRIME, NT::FATOR};
                break;
            case 34:
                children = {NT::POTENCIA_PRIME, NT::FATOR, Token::Name::POW};
                break;
            case 36:
                children = {Token::Name::PAR_END, NT::EXPRESSAO, Token::Name::PAR_START};
                break;
            case 37:
                children = {Token::Name::ID};
                break;
            case 38:
                children = {Token::Name::CONST};
                break;
            case 39:
                children = {NT::FATOR, Token::Name::SUM};
                break;
            case 40:
                children = {NT::FATOR, Token::Name::SUB};
                break;
            default:
                if (prod > 40)
                    throw -2;
                arvore.next_node()->set_children(std::initializer_list{ArvoreConcreta::epsilon_type()});
                break;
            }

            if (children.size() != 0) {
                arvore.next_node()->set_children(std::ranges::reverse_view(children));
                for (auto child : children)
                    stack.push(child);
            }
        }
    }
    if (nextToken.name != Token::Name::END_OF_FILE)
        throw -3;

    std::cout << arvore.to_string() << std::endl;
}
