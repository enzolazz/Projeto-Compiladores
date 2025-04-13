#pragma once

class SyntacticAnalyzer {
    constexpr static auto TABLE_LINES = 22, TABLE_COLUMNS = 27;
    short table[TABLE_LINES][TABLE_COLUMNS];

 public:
    SyntacticAnalyzer();

    struct T {
        enum {
            PROGRAMA = 0,
            IF,
            THEN,
            ELSE,
            ELSEIF,
            WHILE,
            DO,
            ATTRIBUTION,
            BLOCO_START,
            BLOCO_END,
            END_SENTENCE,
            COLON,
            COMMA,
            PAR_START,
            PAR_END,
            BRACKET_START,
            BRACKET_END,
            ID,
            CONST,
            RELOP,
            TYPE,
            SUM,
            SUB,
            MUL,
            DIV,
            POW,
            END_OF_FILE
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