#pragma once
#include "token.hpp"
#include <memory>
#include <unordered_map>


class Token;


struct InterpreterError: public std::runtime_error
{
    InterpreterError(const std::string & what): std::runtime_error(what) {}
};


struct UndefinedVariable: public InterpreterError
{
    UndefinedVariable(const Token & token): InterpreterError("Undefined variable: " + token.toString()) {}
};


class Interpreter
{
public:
    using Stack = std::vector<Token>;
    using Lookup = std::unordered_map<std::string, Token >;

    Interpreter();

    void push(Token token);

    const Stack & stack() const { return stack_; }

private:

    Token pop_();

    Stack stack_;
    Lookup lookup_;
};
