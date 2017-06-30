#include "token.hpp"

#include <unordered_map>


const std::vector<std::string> Token::TypeNames {
    "NAME",
    "INTEGER",
    "OPERATOR",
    "PLUS",
    "MINUS",
    "TIMES",
    "DIVIDE",
    "LIST_OPEN",
    "LIST_CLOSE",
    "FUNCTION_OPEN",
    "FUNCTION_CLOSE",
    "ASSIGN"
};

Token::Token(Token::Type type, const std::__cxx11::string &string)
    : type_(type)
{
    switch(type) {

    case Type::NAME:
        string_ = string;
        break;

    case Type::INTEGER: {
        std::stringstream ss;
        ss << string;
        ss >> integer_;
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
        };
        type_ = OperatorTypes.at(string);
        break;
    }
    }
}

std::__cxx11::string Token::toString() {
    switch(type_) {

    case Type::NAME:
        return string_;

    case Type::INTEGER: {
        return std::to_string(integer_);
    }
    default:
        return typeName();

    }
}
