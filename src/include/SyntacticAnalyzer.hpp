#pragma once

#include "Lexer.hpp"
#include "Token.hpp"

#include <ArvoreConcreta.hpp>

#define tokred(token) token = static_cast<int>(Token::Name::token)

class SyntacticAnalyzer {
    void figurativamenteQualquerCoisa(auto a);

    constexpr static auto TABLE_LINES = 22, TABLE_COLUMNS = 27;
    short table[TABLE_LINES][TABLE_COLUMNS];
    Lexer lexer;
    ArvoreConcreta arvore;

 public:
    SyntacticAnalyzer(std::ifstream &source);
    void literalmenteQualquerCoisa();

    struct T {
        enum {
            tokred(PROGRAMA),
            tokred(IF),
            tokred(THEN),
            tokred(ELSE),
            tokred(ELSEIF),
            tokred(WHILE),
            tokred(DO),
            tokred(ATTRIBUTION),
            tokred(BLOCO_START),
            tokred(BLOCO_END),
            tokred(END_SENTENCE),
            tokred(COLON),
            tokred(COMMA),
            tokred(PAR_START),
            tokred(PAR_END),
            tokred(BRACKET_START),
            tokred(BRACKET_END),
            tokred(ID),
            tokred(CONST),
            tokred(RELOP),
            tokred(TYPE),
            tokred(SUM),
            tokred(SUB),
            tokred(MUL),
            tokred(DIV),
            tokred(POW),
            tokred(END_OF_FILE)
        };
    };

    struct NT {
        enum {
            PROGRAMA = 0,
            BLOCO,
            DECLARACOES,
            DECLARACAO,
            IDS,
            LISTA_IDS,
            COMANDOS,
            COMANDOS_OPT,
            COMANDO,
            CMD_ATRIBUICAO,
            CMD_SELECAO,
            CMD_ELSE_OPT,
            CMD_REPETICAO,
            CMD_OU_BLOCO,
            CONDICAO,
            EXPRESSAO,
            EXPRESSAO_PRIME,
            TERMO,
            TERMO_PRIME,
            POTENCIA,
            POTENCIA_PRIME,
            FATOR
        };
    };
};
