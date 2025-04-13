#include "SyntacticAnalyzer.hpp"
#include <algorithm>
#include <fstream>
#include <stack>
#include <variant>

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
    std::stack<std::variant<Token::Name, int>> stack;
    stack.push(NT::PROGRAMA);

    auto nextToken = lexer.next_token();

    while (!stack.empty()) {
        auto X = stack.top();

        if (auto v = std::get_if<Token::Name>(&X)) {
            if (*v == nextToken.name) {
                stack.pop();
                nextToken = lexer.next_token();
            } else
                throw 0;
        } else {
            int nt = *std::get_if<int>(&X);
            int prod = table[nt][static_cast<int>(nextToken.name)];
            if (prod == -1)
                throw -1;

            // figurativamenteQualquerCoisa(a);
            stack.pop();
            switch (prod) {
            case 1:
                stack.push(NT::BLOCO);
                stack.push(Token::Name::PAR_END);
                stack.push(Token::Name::PAR_START);
                stack.push(Token::Name::ID);
                stack.push(Token::Name::PROGRAMA);
                break;
            case 2:
                stack.push(Token::Name::BRACKET_END);
                stack.push(NT::COMANDOS);
                stack.push(NT::DECLARACOES);
                stack.push(Token::Name::BRACKET_START);
                break;
            case 3:
                stack.push(NT::DECLARACAO);
                stack.push(NT::DECLARACOES);
                break;
            case 5:
                stack.push(Token::Name::END_SENTENCE);
                stack.push(NT::IDS);
                stack.push(Token::Name::COLON);
                stack.push(Token::Name::TYPE);
                break;
            case 6:
                stack.push(NT::LISTA_IDS);
                stack.push(Token::Name::ID);
                break;
            case 7:
                stack.push(NT::LISTA_IDS);
                stack.push(Token::Name::ID);
                stack.push(Token::Name::COLON);
                break;
            case 9:
                stack.push(NT::COMANDOS_OPT);
                stack.push(NT::COMANDO);
                break;
            case 10:
                stack.push(NT::COMANDOS);
                break;
            case 12:
                stack.push(NT::CMD_ATRIBUICAO);
                break;
            case 13:
                stack.push(NT::CMD_SELECAO);
                break;
            case 14:
                stack.push(NT::CMD_REPETICAO);
                break;
            case 15:
                stack.push(Token::Name::END_SENTENCE);
                stack.push(NT::EXPRESSAO);
                stack.push(Token::Name::ATTRIBUTION);
                stack.push(Token::Name::ID);
                break;
            case 16:
                stack.push(NT::CMD_ELSE_OPT);
                stack.push(NT::CMD_OU_BLOCO);
                stack.push(Token::Name::THEN);
                stack.push(Token::Name::BRACKET_END);
                stack.push(NT::CONDICAO);
                stack.push(Token::Name::BRACKET_START);
                stack.push(Token::Name::IF);
                break;
            case 17:
                stack.push(NT::CMD_ELSE_OPT);
                stack.push(NT::CMD_OU_BLOCO);
                stack.push(Token::Name::THEN);
                stack.push(Token::Name::BRACKET_END);
                stack.push(NT::CONDICAO);
                stack.push(Token::Name::BRACKET_START);
                stack.push(Token::Name::ELSEIF);
                break;
            case 18:
                stack.push(NT::CMD_OU_BLOCO);
                stack.push(Token::Name::ELSE);
                break;
            case 20:
                stack.push(NT::CMD_OU_BLOCO);
                stack.push(Token::Name::DO);
                stack.push(Token::Name::BRACKET_END);
                stack.push(NT::CONDICAO);
                stack.push(Token::Name::BRACKET_START);
                stack.push(Token::Name::WHILE);
                break;
            case 21:
                stack.push(Token::Name::END_SENTENCE);
                stack.push(Token::Name::BRACKET_END);
                stack.push(NT::CONDICAO);
                stack.push(Token::Name::BRACKET_START);
                stack.push(Token::Name::WHILE);
                stack.push(NT::CMD_OU_BLOCO);
                stack.push(Token::Name::DO);
                break;
            case 22:
                stack.push(NT::COMANDO);
                break;
            case 23:
                stack.push(NT::BLOCO);
                break;
            case 24:
                stack.push(NT::EXPRESSAO);
                stack.push(Token::Name::RELOP);
                stack.push(NT::EXPRESSAO);
                break;
            case 25:
                stack.push(NT::EXPRESSAO_PRIME);
                stack.push(NT::TERMO);
                break;
            case 26:
                stack.push(NT::EXPRESSAO_PRIME);
                stack.push(NT::TERMO);
                stack.push(Token::Name::SUM);
                break;
            case 27:
                stack.push(NT::EXPRESSAO_PRIME);
                stack.push(NT::TERMO);
                stack.push(Token::Name::SUB);
                break;
            case 29:
                stack.push(NT::TERMO_PRIME);
                stack.push(NT::POTENCIA);
                break;
            case 30:
                stack.push(NT::TERMO_PRIME);
                stack.push(NT::POTENCIA);
                stack.push(Token::Name::MUL);
                break;
            case 31:
                stack.push(NT::TERMO_PRIME);
                stack.push(NT::POTENCIA);
                stack.push(Token::Name::DIV);
                break;
            case 33:
                stack.push(NT::POTENCIA_PRIME);
                stack.push(NT::FATOR);
                break;
            case 34:
                stack.push(NT::POTENCIA_PRIME);
                stack.push(NT::FATOR);
                stack.push(Token::Name::POW);
                break;
            case 36:
                stack.push(Token::Name::PAR_END);
                stack.push(NT::EXPRESSAO);
                stack.push(Token::Name::PAR_START);
                break;
            case 37:
                stack.push(Token::Name::ID);
                break;
            case 38:
                stack.push(Token::Name::CONST);
                break;
            case 39:
                stack.push(NT::FATOR);
                stack.push(Token::Name::SUM);
                break;
            case 40:
                stack.push(NT::FATOR);
                stack.push(Token::Name::SUB);
                break;
            default:
                if (prod > 40)
                    throw -2;
                break;
            }
            stack.pop();
        }
    }
    if (nextToken.name != Token::Name::END_OF_FILE)
        throw -3;
}
