#pragma once

enum class Token : short {
    PROGRAMA,
    IF,
    THEN,
    ELSE,
    ELSEIF,
    WHILE,
    DO,
    ATTRIBUTION,
    BLOCO_START, BLOCO_END,
    END_SENTENCE,
    PAR_START, PAR_END,
    BRACKET_START, BRACKET_END,
    ID,
    NUM,
    CARACTERE,
    RELOP,
    TIPO,
    OPSOMASUB,
    OPMULDIV,
    OPPOT
};