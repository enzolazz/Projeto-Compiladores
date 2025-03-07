#pragma once
#include <string>

enum class TokenName : short {
    PROGRAMA,
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
    NUM,
    CARACTERE,
    RELOP,
    TIPO,
    OPSOMASUB,
    OPMULDIV,
    OPPOT
};

struct Token {
    TokenName name;
    void *attribute;
    // union {
    //     std::string LEX;
    //     int INT;
    //     short RELOP;
    //     void *NULLPTR;
    // } attribute;
    int row;
    int col;
};
