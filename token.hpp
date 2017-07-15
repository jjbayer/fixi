#pragma once

#include <sstream>
#include <string>
#include <vector>
#include <iostream>
#include <memory>
#include <unordered_map>


class Stack;
class Lookup;


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
        FUNCTION,
        FUNCTION_CLOSE,
        ASSIGN,
    };

    using Function = std::function<void(Stack&,Lookup&)>;

    static const std::vector<std::string> TypeNames;

    Token(Type type, const std::string & string);

    Token(const Function & function): type_(Type::FUNCTION), function(function)
    {

    }
    Token(int value): type_(Type::INTEGER), integer_(value)
    {

    }
    Token(double value): type_(Type::FLOAT), float_(value)
    {

    }

    Type type() const {return type_; }
    const std::string & typeName() const { return TypeNames[static_cast<int>(type_)]; }
    std::string toString() const;

    int integer() const { return integer_; }
    double floating() const { return float_; }

    Function function;

private:

    Type type_;
    union {
        int integer_;
        double float_;
    };
    std::shared_ptr<std::string> string_;
};
