#pragma once
#include <token.hpp>


template<typename T> T add(T a, T b) { return a + b; }
template<typename T> T sub(T a, T b) { return a - b; }
template<typename T> T mul(T a, T b) { return a * b; }
template<typename T> T div(T a, T b) { return a / b; }


void binop(std::function<int(int,int)> opi,
           std::function<double(double, double)> opf,
           Stack & stack,
           Lookup & lookup)
{
    if(! stack.size() >= 2) throw InterpreterError("+ needs two operands");
    static const auto Integer = Token::Type::INTEGER;
    static const auto Float = Token::Type::FLOAT;
    auto arg1 = stack.pop();
    auto arg2 = stack.pop();

    auto t1 = arg1.type();
    auto t2 = arg2.type();

    if( t1 == Integer && t2 == Integer ) {
        stack.push(opi(arg1.integer(), arg2.integer()));
    } else if( t1 == Integer && t2 == Float ) {
        stack.push(opf(arg1.integer(), arg2.floating()));
    } else if( t1 == Float && t2 == Integer ) {
        stack.push(opf(arg1.floating(), arg2.integer()));
    } else if( t1 == Float && t2 == Float ) {
        stack.push(opf(arg1.floating(), arg2.floating()));
    } else {
        throw InterpreterError("'+' requires arguments to be integer or float");
    }
}
