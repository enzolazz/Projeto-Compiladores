#include "Token.hpp"
#include <any>
#include <stdexcept>
#include <string>

std::string Token::to_string(Name type) {
    switch (type) {
    case Name::PROGRAMA:
        return "PROGRAMA";
    case Name::IF:
        return "IF";
    case Name::THEN:
        return "THEN";
    case Name::ELSE:
        return "ELSE";
    case Name::ELSEIF:
        return "ELSEIF";
    case Name::WHILE:
        return "WHILE";
    case Name::DO:
        return "DO";
    case Name::ATTRIBUTION:
        return "ATTRIBUTION";
    case Name::BLOCO_START:
        return "BLOCO_START";
    case Name::BLOCO_END:
        return "BLOCO_END";
    case Name::END_SENTENCE:
        return "END_SENTENCE";
    case Name::COLON:
        return "COLON";
    case Name::COMMA:
        return "COMMA";
    case Name::PAR_START:
        return "PAR_START";
    case Name::PAR_END:
        return "PAR_END";
    case Name::BRACKET_START:
        return "BRACKET_START";
    case Name::BRACKET_END:
        return "BRACKET_END";
    case Name::ID:
        return "ID";
    case Name::NUM:
        return "NUM";
    case Name::CARACTERE:
        return "CARACTERE";
    case Name::RELOP:
        return "RELOP";
    case Name::TYPE:
        return "TIPO";
    case Name::OPSOMASUB:
        return "OPSOMASUB";
    case Name::OPMULDIV:
        return "OPMULDIV";
    case Name::OPPOT:
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
    if (id >= Name::PROGRAMA && id <= Name::BRACKET_END)
        str += '>';
    else if (id >= Name::ID && id <= Name::CARACTERE)
        str += ", " + std::any_cast<std::string>(attribute) + '>';
    else if (id == Name::RELOP)
        str += ", " + to_string(std::any_cast<RelOp>(attribute)) + '>';
    else if (id == Name::TYPE)
        str += ", " + to_string(std::any_cast<Type>(attribute)) + '>';
    else
        str += ", nao implementado>";

    return str;
}