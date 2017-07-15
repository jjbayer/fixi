#pragma once
#include "token.hpp"
#include "stack.hpp"
#include "lookup.hpp"

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
    Interpreter();

    const Stack & stack() const { return stack_; }

    void push(Token token);

private:

    Stack stack_;
    Lookup lookup_;
    std::shared_ptr<std::vector<Token> > record_;
};
