#include "Token.hpp"
#include "Lexer.hpp"
#include "SymbolTable.hpp"
#include <stdexcept>

std::string Token::to_string(const Name type) {
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
    case Name::CONST:
        return "CONST";
    case Name::RELOP:
        return "RELOP";
    case Name::TYPE:
        return "TIPO";
    case Name::SUM:
        return "SUM";
    case Name::SUB:
        return "SUB";
    case Name::MUL:
        return "MUL";
    case Name::DIV:
        return "DIV";
    case Name::POW:
        return "POW";
    default:
        throw std::invalid_argument("Unknown Token Id");
    }
}

std::string Token::to_string(const RelOp relop) {
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

std::string Token::to_string(const Type type) {
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
    auto str = '<' + to_string(name);
    if (name >= Name::PROGRAMA && name <= Name::BRACKET_END)
        ;
    else if (name == Name::ID || name == Name::CONST) {
        const auto pos = std::any_cast<SymbolTable::size_type>(attribute);
        str += ", " + std::to_string(pos);
    } else if (name == Name::RELOP)
        str += ", " + to_string(std::any_cast<RelOp>(attribute));
    else if (name == Name::TYPE)
        str += ", " + to_string(std::any_cast<Type>(attribute));
    else
        str += ", nao implementado";

    // return str + ", row: " + std::to_string(row) + ", col: " + std::to_string(col) + ">";
    return str + ">";
}
