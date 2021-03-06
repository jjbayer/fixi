#include "token.hpp"

#include <unordered_map>


const std::vector<std::string> Token::TypeNames {
    "NAME",
    "SYMBOL",
    "INTEGER",
    "FLOAT",
    "STRING",
    "OPERATOR",
    "PLUS",
    "MINUS",
    "TIMES",
    "DIVIDE",
    "LIST_OPEN",
    "LIST_SEPARATOR",
    "LIST_CLOSE",
    "FUNCTION_OPEN",
    "FUNCTION",
    "FUNCTION_CLOSE",
    "ASSIGN"
};

Token::Token(Token::Type type, const std::string &string)
    : type_(type)
{
    switch(type) {

    case Type::NAME:
        string_.reset(new std::string(string));
        break;

    case Type::INTEGER: {
        std::stringstream ss;
        ss << string;
        ss >> integer_;
        break;
    }

    case Type::FLOAT: {
        std::stringstream ss;
        ss << string;
        ss >> float_;
        break;
    }
    case Type::STRING: {
        string_.reset(new std::string(string));
        break;
    }

    case Type::SYMBOL: {
        string_.reset(new std::string(string));
        break;
    }

    case Type::OPERATOR:
    {
        static const std::unordered_map<std::string, Type> OperatorTypes {
            {"+", Type::PLUS},
            {"-", Type::MINUS},
            {"*", Type::TIMES},
            {"/", Type::DIVIDE},
            {"(", Type::LIST_OPEN},
            {")", Type::LIST_CLOSE},
            {"=", Type::ASSIGN},
            {",", Type::LIST_SEPARATOR},
            {"\n", Type::LIST_SEPARATOR}
        };
        type_ = OperatorTypes.at(string);
        break;
    }
    }
}

std::string Token::toString() const {
    switch(type_) {

    case Type::NAME:
        return *string_;

    case Type::SYMBOL:
        return *string_;

    case Type::INTEGER: {
        return std::to_string(integer_);
    }
    case Type::FLOAT: {
        return std::to_string(float_);
    }
    case Type::STRING: {
        return "'" + *string_ + "'";
    }
    default:
        return typeName();

    }
}
