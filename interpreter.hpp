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

    Interpreter()
    {
        lookup_.emplace("+", Token([](Stack & stack, Lookup & lookup) {
                                if(! stack.size() >= 2) throw InterpreterError("+ needs two operands");
                                auto summand2 = *stack.rbegin();
                                stack.pop_back();
                                auto summand1 = *stack.rbegin();
                                stack.pop_back();
                                if(summand1.type() == summand2.type()) {
                                    if(summand1.type() == Token::Type::INTEGER) {
                                        stack.push_back( Token(summand1.integer() + summand2.integer()) );
                                    } else if(summand1.type() == Token::Type::FLOAT) {
                                        stack.push_back( Token(summand1.floating() + summand2.floating()) );
                                    } else {
                                        throw InterpreterError("+ needs int or float");
                                    }
                                } else {
                                    throw InterpreterError("Can only sum two ints or two floats (for now)");
                                }
                            })
                        );
    }

    void push(Token token);

    const Stack & stack() const { return stack_; }

private:
    Stack stack_;
    Lookup lookup_;
};
