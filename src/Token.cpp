#include "Token.hpp"
#include <any>
#include <stdexcept>
#include <string>

std::string Token::to_string(Id type) {
    switch (type) {
    case Id::PROGRAMA:
        return "PROGRAMA";
    case Id::IF:
        return "IF";
    case Id::THEN:
        return "THEN";
    case Id::ELSE:
        return "ELSE";
    case Id::ELSEIF:
        return "ELSEIF";
    case Id::WHILE:
        return "WHILE";
    case Id::DO:
        return "DO";
    case Id::ATTRIBUTION:
        return "ATTRIBUTION";
    case Id::BLOCO_START:
        return "BLOCO_START";
    case Id::BLOCO_END:
        return "BLOCO_END";
    case Id::END_SENTENCE:
        return "END_SENTENCE";
    case Id::COLON:
        return "COLON";
    case Id::COMMA:
        return "COMMA";
    case Id::PAR_START:
        return "PAR_START";
    case Id::PAR_END:
        return "PAR_END";
    case Id::BRACKET_START:
        return "BRACKET_START";
    case Id::BRACKET_END:
        return "BRACKET_END";
    case Id::ID:
        return "ID";
    case Id::NUM:
        return "NUM";
    case Id::CARACTERE:
        return "CARACTERE";
    case Id::RELOP:
        return "RELOP";
    case Id::TYPE:
        return "TIPO";
    case Id::OPSOMASUB:
        return "OPSOMASUB";
    case Id::OPMULDIV:
        return "OPMULDIV";
    case Id::OPPOT:
        return "OPPOT";
    default:
        throw std::invalid_argument("Unknown Token Id");
    }
}

std::string Token::to_string(RelOp relop) {
    switch (relop) {
    case RelOp::LT:
        return "LT";
    case RelOp::LE:
        return "LE";
    case RelOp::EQ:
        return "EQ";
    case RelOp::NE:
        return "NE";
    case RelOp::GE:
        return "GE";
    case RelOp::GT:
        return "GT";
    default:
        throw std::invalid_argument("Unknown RelOp");
    }
}

std::string Token::to_string(Type type) {
    switch (type) {
    case Type::INT:
        return "INT";
    case Type::FLOAT:
        return "FLOAT";
    case Type::CHAR:
        return "CHAR";
    default:
        throw std::invalid_argument("Unknown Type");
    }
}

std::string Token::to_string() const {
    auto str = '<' + Token::to_string(id);
    if (id >= Id::PROGRAMA && id <= Id::BRACKET_END)
        str += '>';
    else if (id >= Id::ID && id <= Id::CARACTERE)
        str += ", " + std::any_cast<std::string>(attribute) + '>';
    else if (id == Id::RELOP)
        str += ", " + to_string(std::any_cast<RelOp>(attribute)) + '>';
    else if (id == Id::TYPE)
        str += ", " + to_string(std::any_cast<Type>(attribute)) + '>';
    else
        str += ", nao implementado>";

    return str;
}