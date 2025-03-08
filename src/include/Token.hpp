#pragma once

#include <any>
#include <string>

struct Token {
    enum class Name : short {
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
        OPPOT,
        PAR_START,
        PAR_END,
        BRACKET_START,
        BRACKET_END,
        ID,
        NUM,
        CARACTERE,
        RELOP,
        TYPE,
        OPSOMASUB,
        OPMULDIV
    };
    enum class RelOp : short { LT, LE, EQ, NE, GE, GT };
    enum class Type : short { INT, FLOAT, CHAR };

    static std::string to_string(Name);
    static std::string to_string(RelOp);
    static std::string to_string(Type);
    std::string to_string() const;

    Name id;
    std::any attribute;
    int row;
    int col;
};
