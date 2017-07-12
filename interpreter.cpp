#include "interpreter.hpp"
#include "token.hpp"

//struct Unde

Interpreter::Interpreter()
{
    lookup_.emplace("+", [&](Stack & stack, Lookup & lookup) {
                        if(! stack.size() >= 2) throw InterpreterError("+ needs two operands");
                        static const auto Integer = Token::Type::INTEGER;
                        static const auto Float = Token::Type::FLOAT;
                        auto arg1 = pop_();
                        auto arg2 = pop_();
                        if(arg2.type() < arg1.type()) {
                            std::swap(arg1, arg2);
                        }
                        auto t1 = arg1.type();
                        auto t2 = arg2.type();

                        if( t1 == Integer ) {
                            if( t2 == Integer ) {
                                stack.push_back(Token(arg1.integer() + arg2.integer()));
                            } else if( t2 == Float ) {
                                stack.push_back(Token(arg1.integer() + arg2.floating()));
                            } else {
                                throw InterpreterError("'+' requires arguments to be integer or float");
                            }
                        } else if ( t1 == Float && t2 == Float ){
                                stack.push_back(Token(arg1.floating() + arg2.floating()));
                        } else {
                            throw InterpreterError("'+' requires arguments to be integer or float");
                        }

                    }
            );
    lookup_.emplace("*", [&](Stack & stack, Lookup & lookup) {
                        if(! stack.size() >= 2) throw InterpreterError("'*' needs two operands");
                        static const auto Integer = Token::Type::INTEGER;
                        static const auto Float = Token::Type::FLOAT;
                        auto arg1 = pop_();
                        auto arg2 = pop_();
                        if(arg2.type() < arg1.type()) {
                            std::swap(arg1, arg2);
                        }
                        auto t1 = arg1.type();
                        auto t2 = arg2.type();

                        if( t1 == Integer ) {
                            if( t2 == Integer ) {
                                stack.push_back(Token(arg1.integer() * arg2.integer()));
                            } else if( t2 == Float ) {
                                stack.push_back(Token(arg1.integer() * arg2.floating()));
                            } else {
                                throw InterpreterError("'*' requires arguments to be integer or float");
                            }
                        } else if ( t1 == Float && t2 == Float ){
                                stack.push_back(Token(arg1.floating() * arg2.floating()));
                        } else {
                            throw InterpreterError("'*' requires arguments to be integer or float");
                        }

                    }
            );
}

void Interpreter::push(Token token)
{
    switch(token.type()) {
    case Token::Type::NAME: {
        try {
            token = lookup_.at(token.toString());
        } catch(std::out_of_range&e) {
            throw UndefinedVariable(token);
        }

        if(token.type() == Token::Type::FUNCTION) {
            token.function(stack_, lookup_);
        } else {
            stack_.push_back(token);
        }

        break;
    }
    default:

        stack_.push_back(token);
    }
}

Token Interpreter::pop_()
{
    if(stack_.empty()) {
        throw std::runtime_error("Internal error: trying to pop from empty stack");
    }

    auto token = *stack_.rbegin();
    stack_.pop_back();

    return token;
}
