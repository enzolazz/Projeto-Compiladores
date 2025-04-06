#pragma once

#include <any>
#include <string>

struct Token {
    enum class Name : short {
        END_OF_FILE,
        PROGRAMA,
        IF,
        THEN,
        ELSE,
        ELSEIF,
        WHILE,
        DO,
        ATTRIBUTION,
        SUM,
        SUB,
        MUL,
        DIV,
        POW,
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
        TYPE
    };
    enum class RelOp : short { LT, LE, EQ, NE, GE, GT };
    enum class Type : short { INT, FLOAT, CHAR };

    static std::string to_string(Name);
    static std::string to_string(RelOp);
    static std::string to_string(Type);
    std::string to_string() const;

    Name name;
    std::any attribute;
    int row;
    int col;
};
