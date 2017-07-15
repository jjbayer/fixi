#pragma once
#include "token.hpp"
#include "error.hpp"

#include <vector>


struct EmptyStack: public Error
{

};


/// TODO: make thread-safe?
class Stack
{
public:
    void push(Token);

    Token pop();

    int size() const { return data_.size(); }

    const std::vector<Token> & data() const { return data_; }

private:
    std::vector<Token> data_;
};
