#pragma once

#include <sstream>
#include <string>
#include <vector>
#include <iostream>


class Token
{
public:
    enum class Type {
        NAME,
        SYMBOL,
        INTEGER,
        FLOAT,
        STRING,
        OPERATOR,
        PLUS,
        MINUS,
        TIMES,
        DIVIDE,
        LIST_OPEN,
        LIST_SEPARATOR,
        LIST_CLOSE,
        FUNCTION_OPEN,
        FUNCTION_CLOSE,
        ASSIGN,
    };
    static const std::vector<std::string> TypeNames;

    Token(Type type, const std::string & string);

    Type type() const {return type_; }
    const std::string & typeName() const { return TypeNames[static_cast<int>(type_)]; }
    std::string toString();

private:

    Type type_;
    std::string string_;
    int integer_;
    double float_;
};
