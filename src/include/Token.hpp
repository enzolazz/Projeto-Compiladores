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
        ATTRIBUITION,
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
    enum class OpMulDiv : short { MUL, DIV };
    enum class OpSomaSub : short { SOMA, SUB };

    static std::string to_string(Name);
    static std::string to_string(RelOp);
    static std::string to_string(Type);
    static std::string to_string(OpMulDiv);
    static std::string to_string(OpSomaSub);
    std::string to_string() const;

    Name id;
    std::any attribute;
    int row;
    int col;
};
